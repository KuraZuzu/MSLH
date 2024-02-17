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
    constexpr float32_t BATTERY_VOLTAGE_RATIO = (1000.0f + 2000.0f) / 1000.0f;

    /** Weight */
    // constexpr float32_t WEIGHT = 0.5f * 2; //タイヤ空転実験用
    constexpr float32_t WEIGHT = 131.65f; // 131.65[g]
    // constexpr float32_t WEIGHT = 10.0f; // 131.65[g]
    constexpr float32_t GRAVITY = 9.8f;
    constexpr float32_t MASS = WEIGHT / GRAVITY;

    /** Unit is "pulse" */
    constexpr float32_t GEAR_RATIO = 42.0f / 13.0f;
    constexpr int32_t ENCODER_ONE_ROTATION_PULSE = 4096 * 4 * GEAR_RATIO;

    /** Unit is "mm" */
    constexpr float32_t WHEEL_DIAMETER = 24.5f;
    constexpr float32_t WHEEL_RADIUS = WHEEL_DIAMETER / 2.0f;
    constexpr float32_t MACHINE_TREAD = 64.15;  // 72.15 - 8.0
    constexpr float32_t DISTANCE_PER_PULSE = WHEEL_DIAMETER * PI / ENCODER_ONE_ROTATION_PULSE;

    /** P-parameter for motor source voltage */
    constexpr float32_t MAX_VELOCITY = 1000; // [mm/s]
    constexpr float32_t KP_MOTOR_VOLTAGE = 0.2f;  //< P制御ゲイン(単一のwheel回転制御)
    constexpr float32_t KI_MOTOR_VOLTAGE = 0.6f;  //< I制御ゲイン(単一のwheel回転制御)
    constexpr float32_t KD_MOTOR_VOLTAGE = 0.0f;  //< D制御ゲイン(単一のwheel回転制御) 0.00001

    /** Motor */
    constexpr float32_t RESISTANCE_MOTOR = 1.07;  //< [Ω]
    constexpr float32_t K_E = 0.000207f;  //< 逆起電力定数（発生電圧定数）[V/rpm]
    constexpr float32_t K_T = 0.00198f;  //< モータトルク定数[Nm/A]
    //constexpr float32_t K_T = K_E * 60.0f / (2.0f * PI);  //< モータトルク定数[Nm/A]

    /** Machine Control */
    constexpr float32_t KP_STRAIGHT_CONTROL = 10.0f; //< 直進制御のためのP制御ゲイン

    // 直進壁制御用のセンサ値
    constexpr uint16_t LS_CENTER_VALUE = 207;  // 中央にロボットを置いたときに左壁センサ値  207(会場)
    constexpr uint16_t RS_CENTER_VALUE = 362;  // 中央にロボットを置いたときに右壁センサ値  362

    // 壁有無判定の閾センサ値
    constexpr uint16_t LS_TH_VALUE = 112;  // 左壁の有無判定の閾値  240(会場)
    constexpr uint16_t RS_TH_VALUE = 400;  // 右壁の有無判定の閾値  152
    constexpr uint16_t LF_TH_VALUE = 138;  // 前壁左側の有無判定の閾値  138
    constexpr uint16_t RF_TH_VALUE = 170;  // 前壁右側の有無判定の閾値  178

    /** Static */
    constexpr float32_t ONE_BLOCK_DISTANCE = 180.0f;
    constexpr float32_t HALF_BLOCK_DISTANCE = ONE_BLOCK_DISTANCE / 2.0f;
    // constexpr float32_t START_BLOCK_DISTANCE = 22.71f;
    constexpr float32_t TURN_90_DEG_DISTANCE = MACHINE_TREAD * PI / 4.0f;
    constexpr float32_t TURN_180_DEG_DISTANCE = TURN_90_DEG_DISTANCE * 2.0f;

    constexpr float32_t OPEN_FRONT_WALL_THRESHOLD = 80.0f; //< あとから数値入れる [mm] 仮ぎめ
    constexpr float32_t OPEN_SIDE_WALL_THRESHOLD = 30.0f; //< あとから数値入れる [mm] 仮ぎめ

    /** Running */
    constexpr float32_t MIN_SPEED = 100.0f;

    /** Wheel Controle */
    constexpr bool FEED_FORWARD_FLAG = false;
    static bool wall_feedback_flag = false;;
}  // namespace mslh

#endif //MSLH_PARAMETER_H
