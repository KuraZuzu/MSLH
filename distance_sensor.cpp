/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor.h"
#include <utility>


mslh::DistanceSensor::DistanceSensor(DigitalOut &led, AnalogInDMAStream &photo_transistor)
: _led(led), _photo_transistor(photo_transistor) {
}

void mslh::DistanceSensor::init() {
    _led.write(0);
    _photo_transistor.init();
}

uint16_t mslh::DistanceSensor::read() {
    const uint16_t offset_value = _photo_transistor.read();
    _led.write(1);
    const uint16_t peak_value = _photo_transistor.read();
    _led.write(0);
    return (peak_value - offset_value);
 }
