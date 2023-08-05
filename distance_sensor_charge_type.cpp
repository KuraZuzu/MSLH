/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor_charge_type.h"

#include <utility>


mslh::DistanceSensor::DistanceSensor(const DigitalOut& led, AnalogInDMAStream photo_transistor, Timer timer, std::function<float32_t (uint16_t)> approximate_func)
        : _led(led), _photo_transistor(photo_transistor)
        , _timer(timer)
        , _charge_start_time(0)
        , _approximate_func(std::move(approximate_func)) {
}

void mslh::DistanceSensor::init() {
    _photo_transistor.init();
    _timer.init();
    _led.write(0);
    _charge_start_time = _timer.getTime();
}

uint16_t mslh::DistanceSensor::read(const uint32_t charge_time_us) {

    uint16_t peak_value = 0;
    uint16_t temp_value = 0;

    /** コンデンサのチャージ時間が指定より超過していた場合に放電する */
    if(charge_time_us < (_timer.getTime() - _charge_start_time)) {
        _led.write(1);
        _timer.waitMicroSeconds(10);  //< この時間はマジックナンバーだが、恐らくどれでもOK(D-S間の抵抗値によるRC回路の時定数)
        _led.write(0);
        _charge_start_time = _timer.getTime();
    }

    /** 指定時間に他入りていない分をコンデンサにチャージ */
    while((_timer.getTime() - _charge_start_time) < charge_time_us) { //< 指定時間までコンデンサの充電待機
    }
    const uint16_t offset_value = _photo_transistor.read();  //< センサのオフセット値取得

    /** LEDを点灯して計測開始 */
    _led.write(1);
    const uint32_t sampling_start_time = _timer.getTime();
    while((_timer.getTime() - sampling_start_time) <20) {  //< 波形がピークになるのを待つ(12はセンサによる立ち上がり時間マジックナンバー)
        temp_value = _photo_transistor.read();  //< センサのピーク値取得
        if(peak_value < temp_value) peak_value = temp_value;
    }

    /** LEDを消灯してコンデンサへの充電開始 */
    _led.write(0);
    _charge_start_time = _timer.getTime();

    return peak_value - offset_value;
//    return peak_value;    //< デバッグ用
//    return offset_value;  //<デバッグ用
}
