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
    DistanceSensor(TIM_HandleTypeDef& led_htim_x, uint32_t led_channel, AnalogInDMAStream phtr, DigitalOut led)
    :_phtr(phtr), _led(led) {
        _phtr.start();
    }

    void flash() {

    }

    uint16_t getDistance_mm() {
        return convert_12bit_to_mm(_phtr.read());
    }


private:

    uint16_t convert_12bit_to_mm(uint16_t value) {
        value = 0; //ここで距離変換の数式わちゃわちゃ。
        return value;
    }

    AnalogInDMAStream _phtr;
    DigitalOut _led;

};


#endif //ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
