/*!
 * @file distance_sensor_2.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor_2.h"

mslh::DistanceSensor2::DistanceSensor2(GPIO_TypeDef *led_x, uint16_t led_pin, AnalogInDMAStream photo_transistor)
        : _photo_transistor(photo_transistor)
        , _led_x(led_x)
        , _led_pin(led_pin) {

//    _photo_transistor.start();  //< まだstartしてないから読んだらバグる。
//    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_SET);  //< ledは常に点灯させておくことでコンデンサに給電させない。
}

void mslh::DistanceSensor2::start() {
    _photo_transistor.start();
}

uint32_t mslh::DistanceSensor2::read(uint32_t charge_time_ms) const {
    //結局PWMがいいのではないか。 by Fさん。

    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_RESET);;  //< コンデンサ充電開始
    HAL_Delay(charge_time_ms); //< 1m秒が時定数 ( 47u[F] + 20[Ω] ) 63.2%充電 1.5m秒くらい欲しいかも？
    const uint32_t offset_value = _photo_transistor.read();
    HAL_GPIO_WritePin(_led_x, _led_pin, GPIO_PIN_SET);
    HAL_Delay(1);  // < 波形がピークになるのをまつ まだ仮時間

    return _photo_transistor.read() - offset_value;
    // ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
}

uint32_t mslh::DistanceSensor2::getDistance_mm() {
    return convert_12bit_to_mm(_photo_transistor.read());
}

uint32_t mslh::DistanceSensor2::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}