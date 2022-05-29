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

#ifndef MSLH_PARAMETER_H
#define MSLH_PARAMETER_H

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
        constexpr float32_t KP_MOTOR_DUTY = 0.1f / 21.205751f;
        // 分子はduty比率の分解能。例えば、0.001とすれば、duty比率を1000子に分割　
        // 分母の21・・・は、速度の最低単位なのでクラスに組み込める

    }  // namespace machine_parameter


}  // namespace mslh

#endif //MSLH_PARAMETER_H
