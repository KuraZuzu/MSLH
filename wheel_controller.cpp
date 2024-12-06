/*!
 * @file motor_control.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.12
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "wheel_controller.h"

// mslh::WheelController::WheelController(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery, float32_t wheel_diameter, float32_t sampling_time)
//     : _motor(motor)
//     , _encoder(encoder)
//     , _battery(battery)
//     , _target_accel(0.0f)
//     , _duty_ratio(0.0f)
//     , _sampling_time(sampling_time)
//     , _distance_per_pulse(wheel_diameter * PI / static_cast<float32_t>(_encoder.getOneRotationPulse()))
//     , _velocity_per_pulse(_distance_per_pulse / _sampling_time)
//     , _correction_velocity(0.0f)
//     {}
