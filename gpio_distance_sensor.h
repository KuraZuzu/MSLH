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
#include <functional>

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
    GPIODistanceSensor(DigitalOut led, AnalogInDMAStream photo_transistor, std::function<uint16_t(uint16_t)> approximate_func);

    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read(uint32_t charge_time_ms = 1);


private:

    uint16_t convertApproximateDistance(uint16_t sensor_value) {
        return sensor_value * 2;
    }


    DigitalOut _led;
    AnalogInDMAStream _photo_transistor;
};

}  // namespace mslh

#endif //MSLH_GPIO_DISTANCE_SENSOR_H
