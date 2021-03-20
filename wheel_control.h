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
#include "defines.h"

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
class WheelControl : public Motor, public Encoder {

public:
    WheelControl(const Motor& motor, const Encoder& encoder, float32_t wheel_diameter, uint16_t speed_sampling_time);

    /**
     * @fn この関数を１つ上の階層のタイマ割り込み(任意の周期)でに計測しないと動作しない。
     */
    inline void measureSpeed() {
        Encoder::update();
        _speed = _distance_per_pulse * Encoder::getDeltaPulse() * _speed_sampling_time;
    }

    void start();

    void run(int32_t speed_mm_s, int32_t distance_mm);

    void stop();


    int32_t getSpeed() const;

private:


    inline void controlSpeed(int32_t speed) {
//        arm_abs_f32(&_speed, &_abs_speed, 1);
//        arm_abs_f32(&speed, &speed, 1);
//        if(_abs_speed > speed) _duty_ratio *= _accel_duty_ratio;
//        else if(_abs_speed < speed) _duty_ratio *= _decelerate_duty_ratio;

        if(abs(_speed) > abs(speed)) _duty_ratio *= _accel_duty_ratio;
        else if(abs(_speed) < abs(speed)) _duty_ratio *= _decelerate_duty_ratio;

        Motor::update(_duty_ratio);
    }

    float32_t _duty_ratio;
    float32_t _accel_duty_ratio;       //< 1.5
    float32_t _decelerate_duty_ratio;  //<0.75
    int32_t _speed;  //< mm_per_second.
    int32_t _abs_speed;  //< convert _speed to absolute.
    int32_t _speed_sampling_time;
    int32_t _distance_per_pulse;
};


#endif //ZUZUHALFTPPMOD1_WHEEL_CONTROL_H
