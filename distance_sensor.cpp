/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor.h"

DistanceSensor::DistanceSensor(GPIO_TypeDef *led_x, uint16_t led_pin, AnalogInDMAStream phtr)
        : _led_x(led_x)
        , _led_pin(led_pin)
        , _phtr(phtr)
        , _calibration_value(0) {
//    _phtr.start();  //< まだstartしてないから読んだらバグる。
//    calibration();  //< こちらも同様
    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_SET);  //< ledは常に点灯させておくことでコンデンサに給電させない。
}

void DistanceSensor::start() {
    _phtr.start();
}

uint16_t DistanceSensor::read(uint16_t charge_time_ms) {
    //結局PWMがいいのではないか。 Fさん。
    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_RESET);;  //< コンデンサ充電開始

    HAL_Delay(charge_time_ms); //< 1m秒が時定数 ( 47u[F] + 20[Ω] ) 63.2%充電 1.5m秒くらい欲しいかも？
    //HAL_Delay()だと正確な時間が保証されるか不明。

    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_SET);
//    HAL_Delay(1);  // < 波形がピークになるのをまつ まだ仮時間
    return _phtr.read() - _calibration_value;
    // ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
}

void DistanceSensor::calibration() {
    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_RESET);
    HAL_Delay(1000);  //< Delay for the ADC to stabilize at startup.

    _phtr.start();
    for (int i = 0; i < 10; ++i) {
        _calibration_value += _phtr.read();
    }
    _calibration_value /= 10;

    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

uint16_t DistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_phtr.read());
}

uint16_t DistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}