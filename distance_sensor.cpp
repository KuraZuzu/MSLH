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


mslh::DistanceSensor::DistanceSensor(const DigitalOut& led, AnalogInDMAStream photo_transistor)
        : _led(led), _photo_transistor(photo_transistor) {
}

void mslh::DistanceSensor::start() {
    _photo_transistor.start();
}

uint16_t mslh::DistanceSensor::read(const uint32_t charge_time_us) {

    uint16_t peak_value = 0;
    uint16_t temp_value = 0;
    return temp_value;
 }
