/*!
 * @file distance_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_GPIO_DISTANCE_SENSOR_H
#define MSLH_GPIO_DISTANCE_SENSOR_H

#include "arm_math.h"
#include "pwm_out.h"
#include "analog_in_dma_stream.h"
#include "digital_out.h"

namespace mslh {

/**
 * @brief
 *   This is a distance sensor class using Morinaga-type light emitting circuit.  <br>
 *   森永式発光回路を用いた距離センサクラスです．
 */
class GPIODistanceSensor {

public:

    /**
     * @param photo_transistor is Photo-Transistor adc handler.
     */
    GPIODistanceSensor(DigitalOut led, AnalogInDMAStream photo_transistor);


    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    inline uint16_t read(const uint32_t charge_time_ms) {
        const uint16_t offset_value = _photo_transistor.read();
        _led = 0;
        HAL_Delay(charge_time_ms);
        uint16_t peak_value = 0;
        uint32_t tick_start = HAL_GetTick();
        _led = 1;
        while( (HAL_GetTick() - tick_start) < 1) {
            const uint16_t temp_value = _photo_transistor.read();
            if(peak_value < temp_value) peak_value = temp_value;
        }
        return peak_value - offset_value;
    }

private:

    uint16_t getDistance_mm();

    uint16_t convert_12bit_to_mm(uint16_t value);

    DigitalOut _led;
    AnalogInDMAStream _photo_transistor;

};

}  // namespace mslh

#endif //MSLH_GPIO_DISTANCE_SENSOR_H
