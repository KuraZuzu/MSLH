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
        , _target_accel(0.0f)
        , _speed(0.0f)
        , _ideal_speed(0.0)
        , _target_speed(0.0f)
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
     * @note フィードバック制御
     *   PID制御を実行(現在はP制御のみ)
     */
    const float32_t diff_speed = _ideal_speed - _speed; // (目標速度) - (現在速度)　逆進の際は負の値となる
    const float32_t p_control_error = diff_speed * machine_parameter::KP_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)


    /**
     * @note 目標速度を超えていた場合、加速度を0にする
     *   ①(加速度 > 0) && ((目標速度 - 現在速度) > 0) の際に加速度有効
     *   ②(加速度 < 0) && ((目標速度 - 現在速度) < 0) の際に加速度有効
     *   ①と②を満たさない時，加速度を無効(=0)として速度のPID制御のみ行う(現在はP制御のみ)
     */
    if((_target_accel * (_target_speed - _speed)) <= 0.0f) {
        _target_accel = 0.0f;
        _ideal_speed = _target_speed;
    }


    /** 指定された加速度での理想速度算出。次回割り込み時のフィードバック制御で参照する。 */
    _ideal_speed += (_target_accel * _speed_sampling_time);
    if(_ideal_speed > _target_speed) _ideal_speed = _target_speed;
    const float32_t corrected_speed = _ideal_speed + p_control_error; //PIDを足し合わせた速度算出


    /**
     * @note フィードフォワード制御
     *   加速度から必要な出力電圧を算出する(なぜかPIDのゲインによって特定の速度より上がらなくなる)
     *   ① 加速Wheelトルク = (機体質量) * (タイヤ加速度[mm/ss -> m/ss]) * (タイヤ直径 [mm -> m]) / (2 [2つのモータのため]);
     *   ② 定速度Wheel電圧 = Ke(逆起電力定数[V/rpm]) * (PIDを考慮したモータ回転数 [mm/s -> rpm]  ギア比も考慮)
     */
    // ① 加速Wheelトルク
    const float32_t wheel_torque = machine_parameter::MASS * _target_accel * 0.001f * (machine_parameter::WHEEL_RADIUS * 0.001f) * 0.5f; //  *0.001f は[mm]->[m]変換  *0.5f は2つのモータのため
    const float32_t motor_torque = wheel_torque / machine_parameter::GEAR_RATIO; //必要モータトルク
    const float32_t current = motor_torque / machine_parameter::K_T; // モータに必要な電流
    // ② 定速度Wheel電圧
    const float32_t reverse_voltage = (machine_parameter::K_E * (60.0f * corrected_speed) * machine_parameter::GEAR_RATIO) / (PI * machine_parameter::WHEEL_DIAMETER); // 分子の速度の単位を[mm]のままなのは、分母のタイヤ径も[mm]のため
    const float32_t voltage = (machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage); // 必要電圧


    /** バッテリ電圧を考慮したduty比算出 */
    const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
    const float32_t duty_ratio = voltage / battery_voltage;


    /** モータに電圧印加 */
    _motor.update(duty_ratio);
}