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
     * @note 目標速度を超えていた場合、加速度を0にする
     *   ①(加速度 > 0) && ((目標速度 - 現在速度) > 0) の際に加速度計算 OK
     *   ②(加速度 < 0) && ((目標速度 - 現在速度) < 0) の際に加速度計算 OK
     *   ①と②を満たさない時，加速度を0として速度のPID制御のみ行う(現在はP制御のみ)
     */
    if((_accel * (_target_speed - _speed)) < 0) _accel = 0;

    //PID制御のための差分算出
    const float32_t diff_speed = _ideal_speed - _speed; // (目標速度) - (現在速度) motor-duty比調整のP制御のための差分．
    const float32_t voltage_pid = diff_speed * machine_parameter::KP_MOTOR_VOLTAGE; // ゲインをかける

    /**
     * @note フィードフォワード制御
     *   加速度から必要な出力電圧を算出する
     *   必要wheelトルク = (質量) * (加速度) * (直径 [mm -> m]) / (2 [2つのモータのため]);
     *
     */
    const float32_t wheel_torque = machine_parameter::MASS * (_accel * 0.001f) * (machine_parameter::WHEEL_RADIUS * 0.001f) * 0.5f; // *0.5は2つのモータのため
    const float32_t motor_torque = wheel_torque * machine_parameter::GEAR_RATIO; //必要モータトルク
    const float32_t current = motor_torque / machine_parameter::K_M; // モータに必要な電流
    _ideal_speed = _accel + _speed;
    const float32_t reverse_voltage = machine_parameter::K_E * (_ideal_speed * 0.001f / machine_parameter::WHEEL_RADIUS); // 現在の速度+目標加速度で想定される逆起電力算出
    _voltage += machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage; // 必要電圧

    /**
     * @note フィードバック制御
     *   PID制御を実行(現在はP制御のみ)
     */
    _voltage += voltage_pid * _voltage; // ここで前回の速度計測からのフィードバック制御をかける(フィードバック自体は前回のものを参考に補正をかける)

    // バッテリ電圧を考慮したduty比算出
    const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
    //const float32_t battery_voltage = 4.0f;
    const float32_t duty_ratio = _voltage / battery_voltage;

    _motor.update(duty_ratio); // モータに印加

    // debug.  printf()を呼ぶと、外部でDelay()を呼んだ際に抜けられない。
//    printf("volt: %6lf   bat: %6lf\r\n" , _voltage, battery_voltage);
}