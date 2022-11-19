/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "gpio_distance_sensor.h"


mslh::GPIODistanceSensor::GPIODistanceSensor(DigitalOut led, AnalogInDMAStream photo_transistor, Timer timer, std::function<uint16_t(uint16_t)> approximate_func)
        : _led(led), _photo_transistor(photo_transistor), _timer(timer), _approximate_func(approximate_func) {
}

void mslh::GPIODistanceSensor::start() {
    _photo_transistor.start();
    _timer.start();
}

uint16_t mslh::GPIODistanceSensor::read(const uint32_t charge_time_us) {

    uint16_t peak_value = 0;
    uint16_t temp_value = 0;

    /** LEDを消灯してコンデンサにチャージ開始 */
    _led.write(0);
    _timer.waitMicroSeconds(charge_time_us);  //< コンデンサが充電されるのを待つ
    const uint16_t offset_value = _photo_transistor.read();  //< オフセット値取得

    /** LEDを点灯して計測開始 */
    _led.write(1);
    const uint32_t offset_time = _timer.getTime();
    while((_timer.getTime() - offset_time) < 15) {  //< 波形がピークになるのを待つ(15はセンサによるマジックナンバー)
        temp_value = _photo_transistor.read();  //< ピーク値取得
        if(peak_value < temp_value) peak_value = temp_value;
    }

    return peak_value - offset_value;
}
