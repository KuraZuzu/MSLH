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
#include "analog_in_dma_stream.h"
#include "digital_out.h"
#include <functional>

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
    DistanceSensor(DigitalOut &led, AnalogInDMAStream &photo_transistor);

    void init();

    // inline float32_t getDistance(uint32_t charge_time_us) { return _approximate_func(read()); }

    inline uint16_t getTestRawValue(uint32_t charge_time_us) { return read(); }

    inline float32_t getVoltage(const uint32_t charge_time_us) { return 3.3f * static_cast<float32_t>(read()) / 0x0FFF; }


private:

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read();


    DigitalOut &_led;
    AnalogInDMAStream &_photo_transistor;
    std::function<float32_t (uint16_t)> _approximate_func;
};

}  // namespace mslh

#endif //MSLH_DISTANCE_SENSOR_H
