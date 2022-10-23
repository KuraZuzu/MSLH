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

uint16_t mslh::GPIODistanceSensor::read(const uint32_t charge_time_ms) {
    _led.write(0);
    uint16_t peak_value = 0;
    uint32_t tick_start = HAL_GetTick();
    const uint16_t offset_value = _photo_transistor.read();
    _led.write(1);
    while( (HAL_GetTick() - tick_start) < charge_time_ms ) {
        const uint16_t temp_value = _photo_transistor.read();
        if(peak_value < temp_value) peak_value = temp_value;
    }
    return (peak_value - offset_value);
}
