/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "distance_sensor.h"

DistanceSensor::DistanceSensor(PWMOut
                               led,
                               AnalogInDMAStream phtr
)
        : _led(led)
        , _phtr(phtr)
        , _max_value(0)
        , _current_value(0)
        , _previous_value(0)
        , _not_higher_count(0)
        , _offset_value(0) {

    _led.stop();  //< 明示的にLEDストップ
    measureOffset();
    write(1000);  //< Default setting is 1k[Hz].
}

void DistanceSensor::measureOffset() {
    _phtr.start();
    HAL_Delay(1000);  //< Delay for the ADC to stabilize at startup.
    uint64_t average_value = 0;
    for (int i = 0; i < 10; ++i) {
        average_value += _phtr.read();
    }
    average_value /= 10;
    _offset_value = static_cast<uint16_t>(average_value);
}

void DistanceSensor::reset() {
    _max_value = 0;
    _current_value = 0;
    _previous_value = 0;
    _not_higher_count = 0;
}

void DistanceSensor::starFlash() {
    _led.start();
}

void DistanceSensor::stopFlash() {
    _led.stop();
}

void DistanceSensor::write(uint32_t led_hz) {
    _led.period(led_hz);
    _led = 0.5;  //< Set duty ratio.
}

uint16_t DistanceSensor::read() {
    return _max_value - _offset_value;
}

void DistanceSensor::updateValue() {
    _previous_value = _current_value;
    _current_value = _phtr.read();
    int32_t diff_value = static_cast<int32_t >(_current_value) - _previous_value;
    if(_max_value < _current_value) _max_value = _current_value;
    else _not_higher_count++;
}

uint16_t DistanceSensor::getDistance_mm() {
    return convert_12bit_to_mm(_phtr.read());
}

uint16_t DistanceSensor::convert_12bit_to_mm(uint16_t value) {
    value = 0; //ここで距離変換の数式わちゃわちゃ。
    return value;
}


/**
 * @fn
 *   ここでLEDセンサの値更新．
 *   全てのセンサに対して，ここで行う．
 *   バッテリ監視とかでは必要ないよ．
 */
//void DMA2_Stream0_IRQHandler(void){
//    PWMOut lf_led(htim2, TIM_CHANNEL_1);
//    AnalogInDMAStream lf_phtr(hadc1, 1);
//    DistanceSensor lf_sensor(lf_led, lf_phtr);
//    lf_sensor.updateValue();
//}
//
//void HAL_DMA_IRQHandler(&hdma_adc1){
//        PWMOut lf_led(htim2, TIM_CHANNEL_1);
//        AnalogInDMAStream lf_phtr(hadc1, 1);
//        DistanceSensor lf_sensor(lf_led, lf_phtr);
//        lf_sensor.updateValue();
//};
