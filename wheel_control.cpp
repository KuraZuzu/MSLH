/*!
* @file motor_control.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "wheel_control.h"

mslh::WheelControl::WheelControl(Motor &motor, Encoder &encoder, float32_t wheel_diameter, float32_t speed_sampling_time)
        : _motor(motor)
        , _encoder(encoder)
        , _speed(0.f)
        , _target_speed(0.0f)
        , _duty_ratio(0.0f)
        , _speed_sampling_time(speed_sampling_time)
        , _distance_per_pulse(wheel_diameter*PI/static_cast<float32_t>(_encoder.getOneRotationPulse()))
        , _speed_per_pulse(_distance_per_pulse / _speed_sampling_time)
{
}

void mslh::WheelControl::start() {
    _encoder.start();
    _motor.start();
    _motor.update(0);
}

void mslh::WheelControl::stop() {
    _motor.stop();
    _encoder.stop();
}

void mslh::WheelControl::setSpeed(float32_t speed) {
    _target_speed = speed;
}