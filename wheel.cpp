/*!
* @file motor_control.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "wheel.h"

mslh::Wheel::Wheel(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery, float32_t wheel_diameter, float32_t speed_sampling_time)
        : _motor(motor)
        , _encoder(encoder)
        , _battery(battery)
        , _accel(0.0f)
        , _target_accel(0.0f)
        , _speed(0.0f)
        , _ideal_speed(0.0)
        , _target_speed(0.0f)
        , _old_speed(0.0f)
        , _voltage(0.0f)
        , _speed_sampling_time(speed_sampling_time)
        , _distance_per_pulse(wheel_diameter*PI/static_cast<float32_t>(_encoder.getOneRotationPulse()))
        , _speed_per_pulse(_distance_per_pulse / _speed_sampling_time)
{
}

void mslh::Wheel::start() {
    _encoder.start();
    _motor.start();
    _motor.update(0);
    _battery.start();
}

void mslh::Wheel::stop() {
    _motor.stop();
    _encoder.stop();
}

void mslh::Wheel::reset() {
    _motor.update(0.0f);
    _speed = 0.0f;
    _target_accel = 0.0f;
    _ideal_speed = 0.0f;
    _target_speed = 0.0f;
    _encoder.reset();
}


void mslh::Wheel::setSpeed(float32_t accel, float32_t speed) {
    _target_accel = accel;
    _target_speed = speed;
}

void mslh::Wheel::interruptTwoFreedomDegreeControl() {
    /**
     * @note 目標速度を超えていた場合、加速度を0にする
     *   ①(加速度 > 0) && ((目標速度 - 現在速度) > 0) の際に加速度計算 OK
     *   ②(加速度 < 0) && ((目標速度 - 現在速度) < 0) の際に加速度計算 OK
     *   ①と②を満たさない時，加速度を0として速度のPID制御のみ行う(現在はP制御のみ)
     */
    if((_target_accel * (_target_speed - _speed)) <= 0.0f) _target_accel = 0.0f;


    /**
     * @note フィードバック制御
     *   PID制御を実行(現在はP制御のみ)
     */
    const float32_t diff_speed = _ideal_speed - _speed; // (目標速度) - (現在速度)　逆進の際は負の値となる
    const float32_t p_control_error = diff_speed * machine_parameter::KP_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)

    /**
     * ①
     * 実際の目標加速度に対して純粋にPIDの補正値を加算する。
     * 簡潔な式であり、abs()を使わないために計算も早く、この手法を採用したい。だが、不安定な動作となる(原理的には②と同じだがP制御のパラメータのせいか不安定な加減速)。
     * @warning
     * この式の場合、目標速度より遅かった場合と速かった場合にかける補正の値が逆になってしまうという欠点もある。
     * ・(遅)目標速度よりも遅かった -> その分加速度を加算したいが、指令速度より小さい数値での反映となることが判明しているため、PIDの係数は 1.0 より大きくする必要がある
     * ・(速)目標速度よりも速かった -> その分加速度を減産したいが、指令速度より大きい数値での反映となることが判明しているため、PIDの係数は 1.0 より小さくする必要がある
     * そのため、直接電圧値をいじる②のほうが良いのかとも考えられる。
     */
    //const float32_t accel = _target_accel + p_control_error; // p_control_error は speed_p_errorという名前にするとよいかも

    /**②
     * 電圧値 _voltage に直接PIDの補正値を叩き込む方法。
     * できれば①を採用したいが、うまく行かないので一旦こちらで実装。
     */
    _voltage += abs(_voltage) * p_control_error; // PIDの補正値を
    const float32_t accel = _target_accel; //①の式と同じ形式で計算式に代入したいので、一時的な処理。正式に②をさいようするなら直接 _target_accel の代入に変更。


    /**
     * @note フィードフォワード制御
     *   加速度から必要な出力電圧を算出する(なぜかPIDのゲインによって特定の速度より上がらなくなる)
     *   必要wheelトルク = (質量) * (加速度) * (直径 [mm -> m]) / (2 [2つのモータのため]);
     */
    const float32_t wheel_torque = machine_parameter::MASS * (accel * 0.001f * _speed_sampling_time) * (machine_parameter::WHEEL_RADIUS * 0.001f) * 0.5f; // *0.5は2つのモータのため
    const float32_t motor_torque = wheel_torque * machine_parameter::GEAR_RATIO; //必要モータトルク
    const float32_t current = motor_torque / machine_parameter::K_T; // モータに必要な電流
    const float32_t reverse_voltage = machine_parameter::K_E * (60.0f * accel * 0.001f * _speed_sampling_time / (PI * machine_parameter::WHEEL_DIAMETER)); // 現在の速度+目標加速度で想定される逆起電力算出
    _voltage += (machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage); // 必要電圧


    // バッテリ電圧を考慮したduty比算出
    const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
    const float32_t duty_ratio = _voltage / battery_voltage;


    _motor.update(duty_ratio); // モータに印加


    //指定された加速度での次回割り込み時の理想速度算出。次回割り込み時のフィードバック制御で参照する。
    _ideal_speed += (_target_accel * _speed_sampling_time);
}