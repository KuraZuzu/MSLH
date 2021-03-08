/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor.h"


void DistanceSensor::measureOffset() {
    _led = 0;
    HAL_Delay(1000);  //< Delay for the ADC to stabilize at startup.
    _phtr.start();
    uint64_t average_value = 0;
    for (int i = 0; i < 10; ++i) {
        average_value += _phtr.read();
    }
    average_value /= 10;
    _offset_value = static_cast<uint16_t>(average_value);
}

uint16_t DistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_phtr.read());
}

uint16_t DistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}