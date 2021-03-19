/*!
* @file motor_control.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "wheel_control.h"

WheelControl::WheelControl(const Motor& motor, const Encoder& encoder, float32_t wheel_diameter, uint16_t speed_sampling_time)
        : Motor(motor)
        , Encoder(encoder)
        , _speed(0.0)
        , _duty_ratio(0.0)
        , _accel_duty_ratio(1.5)
        , _decelerate_duty_ratio(0.75)
        , _abs_speed(0)
        , _speed_sampling_time(static_cast<int32_t>(speed_sampling_time))
        , _distance_per_pulse(wheel_diameter*PI/Encoder::_one_rotation_pulse)
{}


void WheelControl::start() {
    Motor::start();
    Motor::update(0);
    Encoder::reset();
    Encoder::start();
}

void WheelControl::run(int32_t speed_mm_s, uint32_t distance_mm) {

    // 現在のパルス数を取得
    int64_t offset_total_pulse = getTotalPulse();
    int32_t pulse = 0;  // abs()に突っ込むために int64_t でなく int32_t
    const int32_t distance_pulse = distance_mm / _distance_per_pulse;

    // first default duty ratio. 初速のDuty比. 理想はspeed をおおよそのduty比にする式を入れたい．
    _duty_ratio = 0.5f;
    if(speed_mm_s < 0) _duty_ratio = -0.5f;
    else if(!speed_mm_s) _duty_ratio = 0.0f;

    // 指定の距離分のパルスまで処理．
    while (abs(pulse) < distance_pulse) {
        controlSpeed(speed_mm_s);
        pulse += static_cast<int32_t>(getTotalPulse() - offset_total_pulse);
    }
    Motor::update(0);
}

void WheelControl::stop() {
    Encoder::stop();
    Motor::stop();
}

int32_t WheelControl::getSpeed() const {
    return _speed;
}
