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
    DistanceSensor(DigitalOut led, AnalogInDMAStream phtr)
    : _led(led)
    , _phtr(phtr)
    , _max_value(0)
    , _offset_value(0) {

        measureOffset();
    }

    void measureOffset();


    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read(const uint16_t charge_time_ms = 1) {
        _led = 0;  //< コンデンサ充電開始
        HAL_Delay(charge_time_ms);
        _led = 1;
        HAL_Delay(1);  //< 1mm秒が時定数 ( 47u[F] + 20[Ω] ) 63.2%充電

        return _phtr.read() - _offset_value;
        //ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
    }

private:

    uint16_t getDistance_mm();

    uint16_t convert_12bit_to_mm(uint16_t value);

    DigitalOut _led;
    AnalogInDMAStream _phtr;
    uint16_t _max_value;
    uint16_t _offset_value;  //< 外乱光のオフセット

};


#endif //ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
