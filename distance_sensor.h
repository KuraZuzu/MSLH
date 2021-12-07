/*!
 * @file distance_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_DISTANCE_SENSOR_H
#define MSLH_DISTANCE_SENSOR_H

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
class DistanceSensor {

public:

    /**
     * @param photo_transistor is Photo-Transistor adc handler.
     */
    DistanceSensor(PWMOut led, AnalogInDMAStream photo_transistor);

    inline void interruptSamplingValue() {

    }

    inline void interruptCallbackResetValue() {

    }

    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint32_t read() const;


private:

    uint32_t getDistance_mm();

    uint32_t convert_12bit_to_mm(uint16_t value);

//    TIM_HandleTypeDef &_htim_x;
//    const uint64_t _channel;
    PWMOut _led;
    AnalogInDMAStream _photo_transistor;
};

}  // namespace mslh

#endif //MSLH_DISTANCE_SENSOR_H
