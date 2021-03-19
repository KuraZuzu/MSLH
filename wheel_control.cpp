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
        , _abs_speed(0)
{}


void WheelControl::start() {
    _motor.start();
    _motor.update(0);
    _encoder.start();
    _encoder.reset();
}

void WheelControl::run(int32_t speed_mm_s, uint16_t distance_mm) {

    // 現在のパルス数を取得
//    int64_t offset_rotation = _encoder.getRotationCount();
//    int64_t offset_surplus_pulse = _encoder.getSurplusPulse();
    int64_t offset_total_pulse = _encoder.getTotalPulse();
    int32_t pulse = 0;  // abs()に突っ込むために int64_t でなく int32_t
    const uint16_t distance_pulse = static_cast<float32_t>(distance_mm) / param::DISTANCE_PER_PULSE;  //< キャスト地獄

    // first default duty ratio. 初速のDuty比. 理想はspeed をおおよそのduty比にする式を入れたい．
    _duty_ratio = 0.5f;
    if(speed_mm_s < 0) _duty_ratio = -0.5f;
    else if(!speed_mm_s) _duty_ratio = 0.0f;

    // 指定の距離分のパルスまで処理．
    while (abs(pulse) < distance_pulse) {
        controlSpeed(speed_mm_s);
//        pulse += (_encoder.getRotationCount() - offset_rotation) * param::MES6_x4_PULSE;
//        pulse += _encoder.getSurplusPulse() - offset_surplus_pulse;
        pulse += _encoder.getTotalPulse() - offset_total_pulse;
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

int32_t WheelControl::getSpeed() const {
    return _speed;
}