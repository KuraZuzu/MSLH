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
        , _min_value(UINT16_MAX)
        , _offset_value(UINT16_MAX)
        , _get_flag(false) {
}

void mslh::DistanceSensor::start() {
    _led.start();
    _led.startUpdateEvent();
    _photo_transistor.start();
    HAL_TIM_Base_Start_IT(&_sampling_htim_x);
}

uint16_t mslh::DistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_photo_transistor.read());
}

uint16_t mslh::DistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}