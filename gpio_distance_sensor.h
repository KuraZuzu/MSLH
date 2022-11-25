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
#include "timer.h"
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
    GPIODistanceSensor(const DigitalOut& led, AnalogInDMAStream photo_transistor, Timer timer, std::function<float32_t (uint16_t)> approximate_func);

    void start();

    inline float32_t getDistance(uint32_t charge_time_us) { return _approximate_func(read(charge_time_us)); }

    inline uint16_t getTestRawValue(uint32_t charge_time_us) { return read(charge_time_us); }

    inline float32_t getVoltage(const uint32_t charge_time_us) { return 3.3f * static_cast<float32_t>(read(charge_time_us)) / 0x0FFF; }


private:

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read(uint32_t charge_time_us);


    DigitalOut _led;
    AnalogInDMAStream _photo_transistor;
    Timer _timer;
    uint32_t _charge_start_time;
    std::function<float32_t (uint16_t)> _approximate_func;
};

}  // namespace mslh

#endif //MSLH_GPIO_DISTANCE_SENSOR_H
