/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "gpio_distance_sensor.h"

mslh::GPIODistanceSensor::GPIODistanceSensor(DigitalOut led, AnalogInDMAStream photo_transistor, std::function<uint16_t(uint16_t)> approximate_func)
        : _led(led), _photo_transistor(photo_transistor) {
}

void mslh::GPIODistanceSensor::start() {
    _photo_transistor.start();
}

uint16_t mslh::GPIODistanceSensor::read(const uint32_t charge_time_ms) {
    uint32_t tick_start;
    uint16_t peak_value = 0;

    //LEDを消灯してコンデンサにチャージ開始
    _led.write(0);
    tick_start = HAL_GetTick();
    while((HAL_GetTick() - tick_start) < charge_time_ms) {  //< コンデンサが充電されるのを待つ(時間は要検討)
    }
    const uint16_t offset_value = _photo_transistor.read();  //< オフセット値取得

    //LEDを点灯して計測開始
    _led.write(1);
    tick_start = HAL_GetTick();
    while((HAL_GetTick() - tick_start) < 1) {  //< 波形がピークになるのを待つもっと短くていいのかも
        const uint16_t temp_value = _photo_transistor.read();  //< ピーク値取得
        if(peak_value < temp_value) peak_value = temp_value;
    }

    return convertApproximateDistance(peak_value - offset_value);
}
