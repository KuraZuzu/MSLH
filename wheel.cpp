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
        , _target_speed(0.0f)
        , _duty_ratio(0.0f)
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
    _encoder.reset();
}

void mslh::Wheel::setSpeed(float32_t speed) {
    _target_speed = speed;
}

void mslh::Wheel::setSpeed(float32_t accel, float32_t speed) {
    _accel = accel;
    _target_speed = speed;
}


//void mslh::Wheel::interruptControlSpeed() {
//    const float32_t diff_speed = _target_speed - _speed; // (目標速度) - (現在速度) motor-duty比調整のP制御のための差分．
//    _duty_ratio += diff_speed * machine_parameter::KP_MOTOR_VOLTAGE;
//    _motor.update(_duty_ratio);
//}

void mslh::Wheel::interruptControlSpeed() {
    const float32_t wheel_torque = machine_parameter::MASS * _accel / 2 * machine_parameter::WHEEL_RADIUS; //必要wheelトルク
    const float32_t motor_torque = wheel_torque * machine_parameter::GEAR_RATIO; //必要モータトルク
    const float32_t current = motor_torque / machine_parameter::K_M; // モータに必要な電流
    //↓ここの回転数について、これは目標速度に対する予定回転数を示すのか？
    const float32_t reverse_voltage = machine_parameter::K_E * ((2 * PI * (static_cast<float32_t>(_encoder.getDeltaPulse())/machine_parameter::ENCODER_ONE_ROTATION_PULSE))/_speed_sampling_time);
    const float32_t voltage = machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage; // 必要電圧
    const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
    _duty_ratio += voltage / battery_voltage;
    _motor.update(_duty_ratio);
}
