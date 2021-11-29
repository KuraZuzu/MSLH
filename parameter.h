/*!
 * @file parameter.h v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.10.26
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "defines.h"
#include "arm_math.h"

#ifndef ZUZUHALFTPPMOD1_PARAMETER_H
#define ZUZUHALFTPPMOD1_PARAMETER_H

namespace mslh {


/**
 * @brief Set up your machine's parameters
 */
    namespace machine_parameter {

    // MCU parameters
        constexpr int32_t SPEED_MEASURE_Hz = 100;  //< Setup frequency for measure speed;

    // Test speed parameters
        constexpr float32_t ACCEL = 0.05f;

    // Unit is "pulse"
        constexpr int32_t MES6_x4_PULSE = 500 * 4;

    // Unit is "mm"
        constexpr int32_t ONE_BLOCK = 90;
        constexpr float32_t WHEEL_DIAMETER = 13.5f;
        constexpr float32_t MACHINE_TREAD = 36.2f;  //< (40 - 3.8)
        constexpr int32_t DISTANCE_PER_PULSE = WHEEL_DIAMETER * PI / MES6_x4_PULSE;

    // P-parameter for motor source.
        constexpr float32_t P_MOTOR_SOURCE = 0.5f;

    }  // namespace machine_parameter


}  // namespace mslh

#endif //ZUZUHALFTPPMOD1_PARAMETER_H
