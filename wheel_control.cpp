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
}

void WheelControl::run(int32_t speed_mm_s, uint16_t distance_mm) {
    uint16_t pulse = 0;
    uint16_t distance_pulse = static_cast<float32_t>(distance_mm) / param::DISTANCE_PER_PULSE;  //< キャスト地獄

    // first default duty ratio. 初速のDuty比. 理想はspeed をおおよそのduty比にする式を入れたい．
    _duty_ratio = 0.5f;
    if(speed_mm_s < 0) _duty_ratio = -0.5f;
    else if(!speed_mm_s) _duty_ratio = 0.0f;

//    while(1){}
    // 指定の距離分のパルスまで処理．
    while (pulse < distance_pulse) {
        controlSpeed(speed_mm_s);
        _encoder.update();
        pulse += _encoder.getDeltaPulse();
    }
    _motor.update(0);
    //どうやら、この関数が永遠に完了しないバグ。
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