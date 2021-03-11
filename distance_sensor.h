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
    DistanceSensor(DigitalOut led, AnalogInDMAStream phtr);

    void calibration();

    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read(uint16_t charge_time_ms = 1);

private:

    uint16_t getDistance_mm();

    uint16_t convert_12bit_to_mm(uint16_t value);

    DigitalOut _led;
    AnalogInDMAStream _phtr;
    uint16_t _calibration_value;  //< 外乱光のオフセット
};


#endif //ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
