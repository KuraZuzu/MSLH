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

#include "arm_math.h"
#include "defines.h"
#include "motor.h"
#include "encoder.h"
#include "parameter.h"
#include <cmath>

namespace mslh {

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
    /**
    * @note WheelControl(___ , ___ , float32_t wheel_diameter, uint16_t speed_sampling_time);
    *
    * @param
    *   wheel_diameter     : milli meter [mm].
    *   speed_sampling_time: milli second [ms].
    */
    WheelControl(Motor motor, Encoder encoder, float32_t wheel_diameter, uint16_t speed_sampling_time);

    /**
     * @fn 呼び出し側のプログラムで，この関数をタイマ割り込み(任意の周期)で計測しないと動作しないので注意してください．
     */
    inline void measureSpeed() {
        _encoder.update();
        _speed = _distance_per_pulse * _encoder.getDeltaPulse() * 1000 / _speed_sampling_time;
    }

    void start();

    void run(int32_t speed_mm_s, int32_t distance_mm);

    void stop();

    inline int32_t getSpeed() const { return _speed; }

private:


    inline void controlSpeed(int32_t speed) {
        // 今は仮であり，モータとの電圧特性を考慮した式に変更予定．
        if(abs(_speed) < abs(speed)) _duty_ratio *= _accel_duty_ratio;
        else if(abs(_speed) > abs(speed)) _duty_ratio *= _decelerate_duty_ratio;
        _motor.update(_duty_ratio);
    }

    float32_t _duty_ratio;
    int32_t _speed;  //< [mm/s] mm_per_second.
    Encoder _encoder;
    Motor _motor;
    const float32_t _accel_duty_ratio;       //< 1.5
    const float32_t _decelerate_duty_ratio;  //< 0.75
    const int32_t _speed_sampling_time;      // milli second [ms]
    const int32_t _distance_per_pulse;       // [mm]/[sec]
};

}  // namespace mslh

#endif //ZUZUHALFTPPMOD1_WHEEL_CONTROL_H
