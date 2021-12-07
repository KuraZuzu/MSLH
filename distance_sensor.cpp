/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor.h"

mslh::DistanceSensor::DistanceSensor(PWMOut led, AnalogInDMAStream photo_transistor)
        : _led(led), _photo_transistor(photo_transistor) {
}

void mslh::DistanceSensor::start() {
    _led.start();
    _photo_transistor.start();
}

uint32_t mslh::DistanceSensor::read() const {
    //結局PWMがいいのではないか。 by Fさん。

    return _photo_transistor.read();
    // ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
}

uint32_t mslh::DistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_photo_transistor.read());
}

uint32_t mslh::DistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}