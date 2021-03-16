/*!
 * @file parameter.h v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.10.26
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "tim.h"

#ifndef PARAMETER_H
#define PARAMETER_H

/**
 * @brief Set up your machine's parameters
 */
namespace machine_param {

// MCU parameters
    constexpr uint16_t SPEED_MEASURE_Hz = 100;  //< Setup frequency for measure speed;

// Math parameters
    constexpr float PI = 3.14159f;

// Test speed parameters
    constexpr float ACCEL = 0.05f;

// Unit is "pulse"
    constexpr uint16_t MES6_x4_PULSE = 500 * 4;

// Unit is "mm"
    constexpr uint16_t ONE_BLOCK = 90;
    constexpr float WHEEL_DIAMETER = 13.5f;
    constexpr float MACHINE_TREAD = 36.2f; /*(40 - 3.8)*/
    constexpr float DISTANCE_PER_PULSE = WHEEL_DIAMETER * PI / MES6_x4_PULSE;
}

#endif //PARAMETER_H
