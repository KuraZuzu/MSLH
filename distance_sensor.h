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
    DistanceSensor(GPIO_TypeDef *led_x, uint16_t led_pin, AnalogInDMAStream photo_transistor);

    void calibration();

    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint32_t read(uint32_t charge_time_ms = 1) const;

private:

    uint32_t getDistance_mm();

    uint32_t convert_12bit_to_mm(uint16_t value);

    GPIO_TypeDef *_led_x;
    const uint16_t _led_pin;
    AnalogInDMAStream _photo_transistor;
    uint16_t _calibration_value;  //< 外乱光のオフセット
};

}  // namespace mslh

#endif //MSLH_DISTANCE_SENSOR_H
