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

namespace mslh::machine_parameter {

    /** Battery Voltage Divider */
    constexpr float32_t BATTERY_VOLTAGE_RATIO = (1000.0f + 1000.0f) / 1000.0f;

    /** Weight */
    constexpr float32_t WEIGHT = 0.1f; //16[g]
    constexpr float32_t GRAVITY = 9.8f;
    constexpr float32_t MASS = WEIGHT / GRAVITY;

    /** Unit is "pulse" */
    constexpr int32_t ENCODER_ONE_ROTATION_PULSE = 500 * 4;
    constexpr float32_t GEAR_RATIO = 9.0f / 39.0f;

    /** Unit is "mm" */
    constexpr float32_t WHEEL_DIAMETER = 13.5f;
    constexpr float32_t WHEEL_RADIUS = WHEEL_DIAMETER / 2.0f;
    constexpr float32_t MACHINE_TREAD = 36.2f;  //< (40 - 3.8)
    constexpr int32_t DISTANCE_PER_PULSE = WHEEL_DIAMETER * PI / ENCODER_ONE_ROTATION_PULSE;

    /** P-parameter for motor source voltage */
    constexpr float32_t MAX_SPEED = 5000; // [mm/s]
    constexpr float32_t MOTOR_VOLTAGE = 3.8f; // [v]
    constexpr float32_t KP_MOTOR_VOLTAGE = MOTOR_VOLTAGE/MAX_SPEED/500.0f;  // (モータ電圧]) / (無負荷時回転数)

    /** Motor */
    constexpr float32_t RESISTANCE_MOTOR = 4.5;  //< [Ω]
    constexpr float32_t K_M = 8.96e-5;  //< モータトルク[Nm/A]
    constexpr float32_t K_E = 2.0f * PI / 60.0f * K_M;  //< 逆起電力定数（発生電圧定数）[V/rpm]


//    /** Unit is "pulse" */
//    constexpr int32_t ENCODER_ONE_ROTATION_PULSE = 500*6*23*4;
//
//    /** Unit is "mm" */
//    constexpr float32_t WHEEL_DIAMETER = 307.0f;
//    constexpr float32_t MACHINE_TREAD = 620.0f;  //< (40 - 3.8)
//    constexpr int32_t DISTANCE_PER_PULSE = WHEEL_DIAMETER * PI / ENCODER_ONE_ROTATION_PULSE;
//
//    /** P-parameter for motor source voltage. */
//    constexpr float32_t MAX_SPEED = 5000; // [mm/s]
//    constexpr float32_t MOTOR_VOLTAGE = 24.0; // [v]
//    constexpr float32_t KP_MOTOR_VOLTAGE = MOTOR_VOLTAGE/MAX_SPEED/500.0f;  // (モータ電圧]) / (無負荷時回転数)

}  // namespace mslh

#endif //MSLH_PARAMETER_H
