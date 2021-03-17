/*!
* @file motor_control.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_WHEEL_CONTROL_H
#define ZUZUHALFTPPMOD1_WHEEL_CONTROL_H

#include "motor.h"
#include "encoder.h"
#include "parameter.h"
#include <cmath>
//#include "cmsis_version.h"
//#include "cmsis_gcc.h"
//#include "cmsis_compiler.h"

namespace param = machine_param;


/**
 * @warning
 *     生成した WheelControlクラス のインスタンス全てに対して，
 *     measureSpeed() をタイマ割り込みで呼び出す必要がある．
 *
 * Example:
 * @code
 *     WheelControl left_wheel(省略);
 *     WheelControl right_wheel(省略);
 *
 *
 *     // 90M[Hz](APB1) / 36000 = 2500[Hz](TIM6)
 *     // 2500[Hz] / 25(Period) = 100[Hz]
 *     // 0.01秒ごとに呼び出される．コールバック関数．
 *     void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
 *     if(htim == &htim6) {
 *         left_wheel.measureSpeed();
 *         right_wheel.measureSpeed();
 *     }
 * }
 * @endcode
 */
class WheelControl {

public:
    WheelControl(Motor motor, Encoder encoder);

    /**
     * @fn この関数を１つ上の階層のタイマ割り込み(任意の周期)でに計測しないと動作しない。
     */
    void measureSpeed();

    void start();

    void goStraight(float speed, float distance);

    void stop();

    int64_t getRotationState();

    int16_t getSpeed();

private:

    void controlSpeed(float speed);


    Motor _motor;
    Encoder _encoder;
    float _duty_ratio;
    float _accel_duty_ratio;       //< 1.5
    float _decelerate_duty_ratio;  //<0.75
    float _speed;  //< mm_per_second.
};


#endif //ZUZUHALFTPPMOD1_WHEEL_CONTROL_H
