/*!
 * @file distance_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
#define ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H

#include "stm32f4xx_it.h"
#include "pwm_out.h"
#include "analog_in_dma_stream.h"
#include "digital_out.h"

/**
 * @brief
 *   This is a distance sensor class using Morinaga-type light emitting circuit.  <br>
 *   森永式発光回路を用いた距離センサクラスです．
 */
class DistanceSensor {

public:

    /**
     * @param phtr is Photo-Transistor adc handler.
     */
    DistanceSensor(PWMOut led, AnalogInDMAStream phtr);

    void measureOffset();

    /**
     * @fn Reset phtr value.
     */
    void reset();

    void starFlash();

    void stopFlash();

    void write(uint32_t led_hz);

    uint16_t read();

    /**
     * @fn
     *   この関数を適宜自分のマシンで用いる ADC+DMA のコールバック関数に組み込んでください．
     *   (コールバック関数をメンバにできない上に，使うDMAによって引数ではなく，コールバック関数名が変わってしまうため。)
     *      (↑ 実は引数で呼び出せるかも．)
     *
     *   void DMA2_Stream0_IRQHandler(void){
     *
     * @example
     *   void DMA2_Stream0_IRQHandler(void){
     *
     *       WMOut lf_led(htim2, TIM_CHANNEL_2);
     *       AnalogInDMAStream lf_phtr(hadc1, 1);
     *
     *       DistanceSensor lf_sensor(lf_led, lf_phtr);
     *       lf_sensor.updateValue();
     *   }
     *
     */
    void updateValue();


private:

    uint16_t getDistance_mm();

    uint16_t convert_12bit_to_mm(uint16_t value);

    PWMOut _led;
    AnalogInDMAStream _phtr;
    uint16_t _max_value;
    uint16_t _current_value;
    uint16_t _previous_value;
    uint16_t _offset_value;  //< 外乱光のオフセット
    uint16_t _not_higher_count;  //< _max_value が更新されない回数。

};


#endif //ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
