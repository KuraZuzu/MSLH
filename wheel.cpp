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
    _accel = 0.0f;
    _ideal_speed = 0.0f;
    _target_speed = 0.0f;
    _encoder.reset();
}

void mslh::Wheel::setSpeed(float32_t speed) {
    _target_speed = speed;
}

void mslh::Wheel::setSpeed(float32_t accel, float32_t speed) {
    _accel = accel;
    _target_speed = speed;
}

void mslh::Wheel::interruptTwoFreedomDegreeControl() {

    /**
     * @note 実加速度_accel計測
     * @warning 今後の並進制御の方法によっては不要となる
     */
//    _accel = (_speed - _old_speed) / _speed_sampling_time; // 実加速度_accel計測
//    _old_speed = _speed; // 実加速度_accel計測のための前回の割り込み時の速度コピー

    /**
     * @note 目標速度を超えていた場合、加速度を0にする
     *   ①( (加速度 > 0) && ((目標速度 - 現在速度) > 0) ) の際に加速度有効
     *   ②( (加速度 < 0) && ((目標速度 - 現在速度) < 0) ) の際に加速度有効
     *   ①と②を満たさない時，加速度無効 (0) として速度のPID制御のみ行う(現在はP制御のみ)
     */
    if((_accel * (_target_speed - _speed)) <= 0.0f) _accel = 0.0f;


    /**
     * @note フィードバック制御
     *   PID制御で目標回転数への補数を算出
     *   フィードフォワードより前に来てるが、１回の割り込み分の遅れとなるため
     *   フィードフォワード -> フィードバックの制御となっている
     */
    const float32_t diff_speed = _ideal_speed - _speed; // (現在の理想速度) - (現在の実速度) motor-duty比調整のP制御のための差分．
    const float32_t pid_error = diff_speed * machine_parameter::KP_MOTOR_VOLTAGE; // ゲインをかける
    _voltage += _voltage * pid_error; // ここで前回の速度計測からのフィードバック(P制御)をかける(フィードバック自体は前回のものを参考に補正をかける)


    //指令通りの加速度による理想速度を計算(１つ前の割り込み時の速度フェイードバックなので、"フィードバック制御"より後に算出)
//    _ideal_speed += (_accel * _speed_sampling_time);

    /**
     * @note フィードフォワード制御
     *   加速度から必要な出力電圧を算出する(なぜかPIDのゲインによって特定の速度より上がらなくなる)
     *   必要wheelトルク = (質量) * (加速度) * (直径 [mm -> m]) / (2 [2つのモータのため]);
     */
    const float32_t wheel_torque = machine_parameter::MASS * (_accel * 0.001f * _speed_sampling_time) * (machine_parameter::WHEEL_RADIUS * 0.001f) * 0.5f; // *0.5は2つのモータのため *0.001は[m]から[mm]への変換
    const float32_t motor_torque = wheel_torque * machine_parameter::GEAR_RATIO; //必要モータトルク
    const float32_t current = motor_torque / machine_parameter::K_T; // モータに必要な電流
    _ideal_speed += (_accel * _speed_sampling_time);
    const float32_t reverse_voltage = machine_parameter::K_E * (60.0f * _accel * 0.001f * _speed_sampling_time / (PI * machine_parameter::WHEEL_DIAMETER)); // 加速度を加算して１割り込み後の回転速度での算出
    _voltage += machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage; // (トルク要求電流値 * モータ抵抗) + モータ逆起電力


    // [バッテリ電圧を考慮したduty比算出]
    //const float32_t battery_voltage = 4.0f; // バッテリ電圧を固定値としたもの
    const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
    const float32_t duty_ratio = _voltage / battery_voltage;

    _motor.update(duty_ratio); // モータに印加
    // [debug]  printf()を呼ぶと、外部でDelay()を呼んだ際に、そのDelay()から抜けられない。
    //printf("%6lf\r\n", _voltage); //重い処理なので数値確認にのみ使用可能

}