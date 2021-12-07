/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor.h"

mslh::DistanceSensor::DistanceSensor(PWMOut led, AnalogInDMAStream photo_transistor, TIM_HandleTypeDef &sampling_htim_x)
        : _led(led)
        , _photo_transistor(photo_transistor)
        , _sampling_htim_x(sampling_htim_x)
        , _value(0)
        , _current_value(0)
        , _previous_value(0)
        , _min_value(UINT32_MAX)
        , _offset_value(UINT32_MAX)
        , _get_flag(false) {
}

void mslh::DistanceSensor::start() {
    _led.start();
    _photo_transistor.start();
//    HAL_TIM_Base_Start_IT(_led.);//PWMのhtimが必要
    HAL_TIM_Base_Start_IT(&_sampling_htim_x);
}


uint32_t mslh::DistanceSensor::read() const {
    while(!_get_flag){} //< 最新の値を持ってくるまで待つ
    return _value;      //< ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
}

uint32_t mslh::DistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_photo_transistor.read());
}

uint32_t mslh::DistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}