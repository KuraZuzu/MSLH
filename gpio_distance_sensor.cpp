/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "gpio_distance_sensor.h"

mslh::GPIODistanceSensor::GPIODistanceSensor(DigitalOut led, AnalogInDMAStream photo_transistor)
        : _led(led)
        , _photo_transistor(photo_transistor) {
}

void mslh::GPIODistanceSensor::start() {
    _photo_transistor.start();
}

uint16_t mslh::GPIODistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_photo_transistor.read());
}

uint16_t mslh::GPIODistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}