/*!
* @file motor_control.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "wheel_control.h"

mslh::WheelControl::WheelControl(Motor motor, Encoder encoder, float32_t wheel_diameter, uint16_t speed_sampling_time)
        : _motor(motor)
        , _encoder(encoder)
        , _speed(0)
        , _duty_ratio(0.0)
        , _speed_sampling_time(static_cast<int32_t>(speed_sampling_time))
        , _distance_per_pulse(wheel_diameter*PI/static_cast<float32_t>(_encoder.getOneRotationPulse()))
        , _speed_per_pulse(_distance_per_pulse * 1000 / static_cast<float32_t>(_speed_sampling_time))
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

//void mslh::WheelControl::run(float32_t speed_mm_s, float32_t distance_mm) {
//    const int64_t offset_total_pulse = _encoder.getTotalPulse();  //< 現在のパルス数取得(オフセット)
//    const int32_t distance_pulse = distance_mm / _distance_per_pulse;  //< 目標パルス数算出
//
//    // first default duty ratio. 初速のDuty比. 理想はspeed をおおよそのduty比にする式を入れたい．
//    if (speed_mm_s > 0) _duty_ratio = 0.5f;
//    if (speed_mm_s < 0) _duty_ratio = -0.5f;
//    else _duty_ratio = 0.0f;
//
//    // 指定の距離分のパルスまで処理．(前転と後転で場合分け．もう少し最適化できそう．)
//    if (distance_pulse > 0) {
//        while (_encoder.getTotalPulse() - offset_total_pulse < distance_pulse) {
//            controlSpeed(speed_mm_s);
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
//        }
//
//    } else if (distance_pulse < 0) {
//        while (_encoder.getTotalPulse() - offset_total_pulse > distance_pulse) {
//            controlSpeed(speed_mm_s);
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
//
//        _motor.update(0);
//    }
//}