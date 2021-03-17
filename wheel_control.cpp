/*!
* @file motor_control.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "wheel_control.h"


WheelControl::WheelControl(Motor motor, Encoder encoder)
        : _motor(motor)
        , _encoder(encoder)
        , _speed(0.0)
        , _duty_ratio(0.0)
        , _accel_duty_ratio(1.5)
        , _decelerate_duty_ratio(0.75)
{}

void WheelControl::measureSpeed() {
    _encoder.update();
    _speed = param::DISTANCE_PER_PULSE * _encoder.getDeltaPulse() * param::SPEED_MEASURE_Hz;
}

void WheelControl::start() {
    _encoder.start();
    _encoder.reset();
    _motor.start();
    _motor.update(0);
}

void WheelControl::goStraight(float32_t speed, float32_t distance) {
    uint16_t pulse = 0;
    uint16_t distance_pulse = distance / param::DISTANCE_PER_PULSE;

    // 指定の距離分のパルスまで処理．
    while (pulse < distance_pulse) {
        controlSpeed(speed);
        pulse += _encoder.getDeltaPulse();
    }
    _motor.update(0);
}

void WheelControl::stop() {
    _encoder.stop();
    _motor.stop();
}

int64_t WheelControl::getRotationState() {
    return _encoder.getRotationCount();
}

int16_t WheelControl::getSpeed() {
    return _speed;
}

void WheelControl::controlSpeed(float32_t speed) {
    if(fabsf(_speed) > fabsf(speed)) _duty_ratio *= _accel_duty_ratio;
    else if(fabsf(_speed) < fabsf(speed)) _duty_ratio *= _decelerate_duty_ratio;

    _motor.update(_duty_ratio);
}
