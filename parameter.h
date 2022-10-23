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
#include <functional>

#ifndef MSLH_PARAMETER_H
#define MSLH_PARAMETER_H

namespace mslh::machine_parameter {

    /** Battery Voltage Divider */
    constexpr float32_t BATTERY_VOLTAGE_RATIO = (1000.0f + 1000.0f) / 1000.0f;

    /** Weight */
//    constexpr float32_t WEIGHT = 0.5f * 2; //17.00[g]
    constexpr float32_t WEIGHT = 17.0f; //17.00[g]
    constexpr float32_t GRAVITY = 9.8f;
    constexpr float32_t MASS = WEIGHT / GRAVITY;

    /** Unit is "pulse" */
    constexpr int32_t ENCODER_ONE_ROTATION_PULSE = 500 * 4;
    constexpr float32_t GEAR_RATIO = 39.0f / 9.0f;

    /** Unit is "mm" */
    constexpr float32_t WHEEL_DIAMETER = 13.5f;
    constexpr float32_t WHEEL_RADIUS = WHEEL_DIAMETER / 2.0f;
    constexpr float32_t MACHINE_TREAD = 36.2f;  //< (40 - 3.8)
    constexpr float32_t DISTANCE_PER_PULSE = WHEEL_DIAMETER * PI / ENCODER_ONE_ROTATION_PULSE;

    /** P-parameter for motor source voltage */
    constexpr float32_t MAX_SPEED = 5000; // [mm/s]
//    constexpr float32_t MOTOR_VOLTAGE = 3.8f; // [v]
    constexpr float32_t KP_MOTOR_VOLTAGE = 0.12f;  //< P制御ゲイン(単一のwheel回転制御)
    constexpr float32_t KI_MOTOR_VOLTAGE = 0.0f;  //< I制御ゲイン(単一のwheel回転制御)
    constexpr float32_t KD_MOTOR_VOLTAGE = 0.0f;  //< D制御ゲイン(単一のwheel回転制御)

    /** Motor */
    constexpr float32_t RESISTANCE_MOTOR = 4.5f;  //< [Ω]
    constexpr float32_t K_E = 0.000062f;  //< 逆起電力定数（発生電圧定数）[V/rpm]
    constexpr float32_t K_T = 0.000594f;  //< モータトルク定数[Nm/A]
//    constexpr float32_t K_T = K_E * 60.0f / (2.0f * PI);  //< モータトルク定数[Nm/A]

    /** Machine Control */
    constexpr float32_t KP_MACHINE_CONTROL = 0.001f; //< 直進制御のためのP制御ゲイン

    /** Distance Sensor */
    uint16_t convertApproximateDistance(u_int16_t sensor_value) {
        return sensor_value;
    }
    uint16_t (*f)(uint16_t);
    std::function<uint16_t(uint16_t)> convert_func = convertApproximateDistance;
//    f = &convertApproximateDistance;


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
