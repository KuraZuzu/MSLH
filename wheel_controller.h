/*!
 * @file motor_control.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.12
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_WHEEL_CONTROLER_H
#define MSLH_WHEEL_CONTROLER_H

#include <cmath>

#include "analog_in_dma_stream.h"
#include "arm_math.h"
#include "defines.h"
#include "encoder.h"
#include "motor.h"
#include "parameter.h"

namespace mslh {
class WheelController {
   public:
    WheelController(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery,
                    float32_t wheel_diameter, float32_t sampling_time)
        : _motor(motor),
          _encoder(encoder),
          _battery(battery),
          _target_accel(0.0f),
          _duty_ratio(0.0f),
          _sampling_time(sampling_time),
          _distance_per_pulse(
              wheel_diameter * PI /
              static_cast<float32_t>(_encoder.getOneRotationPulse())),
          _velocity_per_pulse(_distance_per_pulse / _sampling_time) {}

    void init() {
        _battery.init();
        _encoder.reset();
    }

    void start() {
        _encoder.start();
        _motor.start();
        _motor.update(0.0f);
    }

    void stop() {
        _motor.stop();
        _encoder.stop();
    }

    void reset() { _encoder.reset(); }

    void setVelocity(float32_t velocity) { _target_velocity = velocity; }

    float32_t getVelocity() { return _velocity; }

    void controlWheelISR(float32_t velocity) {
        // ここで速度更新したい
        controlFeedBackISR();
        controlFeedForwardISR();
        _motor.update(_duty_ratio);
        controlMeasureVelocityISR();
    }

   private:
       void controlMeasureVelocityISR() {
        _encoder.update();
        _velocity == _velocity_per_pulse *static_cast<float32_t>(
                         _encoder.getDeltaPulse());
        _target_accel = _velocity - _velocity;
    }

    void controlFeedForwardISR();

    void controlFeedBackISR();

    float32_t _target_accel;
    float32_t _target_velocity;
    float32_t _velocity;
    float32_t _duty_ratio;
    Encoder &_encoder;
    Motor &_motor;
    AnalogInDMAStream &_battery;
    const float32_t _sampling_time;       //< second [s]
    const float32_t _distance_per_pulse;  //< [mm/pulse]
    const float32_t _velocity_per_pulse;  //< [mm/s]

};  // class WheelController

}  // namespace mslh

#endif  // MSLH_WHEEL_CONTROLLER_H
