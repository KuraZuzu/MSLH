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
        : _led(led), _photo_transistor(photo_transistor)
        , _timer(timer)
        , _charge_start_time(0)
        , _approximate_func(approximate_func) {
}

void mslh::GPIODistanceSensor::start() {
    _photo_transistor.start();
    _timer.start();
    _led.write(0);
    _charge_start_time = _timer.getTime();
}

uint16_t mslh::GPIODistanceSensor::read(const uint32_t charge_time_us) {

    uint16_t peak_value = 0;
    uint16_t temp_value = 0;

    /** コンデンサにチャージ */
    while((_timer.getTime() - _charge_start_time) < charge_time_us) {} //< コンデンサの充電がフルになるまで待機
    const uint16_t offset_value = _photo_transistor.read();  //< オフセット値取得

    /** LEDを点灯して計測開始 */
    _led.write(1);
    const uint32_t sampling_start_time = _timer.getTime();
    while((_timer.getTime() - sampling_start_time) < 12) {  //< 波形がピークになるのを待つ(12はセンサによる立ち上がり時間マジックナンバー)
        temp_value = _photo_transistor.read();  //< ピーク値取得
        if(peak_value < temp_value) peak_value = temp_value;
    }
    _led.write(0);
    _charge_start_time = _timer.getTime();

//    return peak_value - offset_value;
    return peak_value;
//    return offset_value;
}
