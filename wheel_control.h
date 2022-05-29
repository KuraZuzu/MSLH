/*!
* @file motor_control.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef MSLH_WHEEL_CONTROL_H
#define MSLH_WHEEL_CONTROL_H

#include "arm_math.h"
#include "defines.h"
#include "motor.h"
#include "encoder.h"
#include "parameter.h"
#include <cmath>

namespace mslh {

/**
 * @warning
 *     For all instances of the generated WheelControl class　
 *     interruptMeasureSpeed() must be called at explicit intervals such as timer interrupt.
 *
 * Example:
 * @code
 *
 *   using namespace mslh;
 *
 *   WheelControl wheel( Motor(htim1, TIM_CHANNEL_1, GPIOC, GPIO_PIN_0, false), Encoder(htim3, 500*4, true), 300, 10);
 *
 *
 *   // Need ticker for measure speed.
 *   // This interrupt intervals are set in .ioc-file (timer.c).
 *   void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
 *
 *       // [ TIM6 (&htim6) ]
 *       // This callback function is Called every 10[ms].
 *       // 90M[Hz](APB1) / 36000 = 2500[Hz]
 *       // 2500[Hz] / 25(Period) = 100[Hz]   -> 10[ms]
 *       if(htim == &htim6) {
 *           wheel.interruptMeasureSpeed();
 *       }
 *
 *
 *   int main() {
 *       // Abbreviation Microcomputer startup settings.
 *
 *       int32_t speed;
 *
 *       MX_TIM1_Init();                 //< Need setup HAL motor timer parameters.
 *       MX_TIM3_Init();                 //< Need setup HAL encoder timer parameters.
 *       MX_GPIO_Init();                 //< Need setup HAL_GPIO for motor _direction.
 *       MX_TIM6_Init();                 //< Need setup HAL ticker(callback) timer parameters.
 *       HAL_TIM_Base_Start_IT(&htim6);  //< Need start ticker(callback) timer parameters.
 *
 *       test.start(3000, 1000000);      //< Need start motor and encoder.
 *       test.run(500);                  //< Running (wheel rotation) at a specified speed[mm/s].
 *
 *       int32_t wheel_speed = test_wheel.getSpeed();  //< measure wheel speed
 *
 *       test.stop();  //< Stop all work (Encoder values are retained).
 *   }
 * @endcode
 */
class WheelControl {

public:
    /**
    * @note WheelControl(___ , ___ , float32_t wheel_diameter, uint16_t speed_sampling_time);
    *
    * @param
    *   wheel_diameter     : milli meter [mm].
    *   speed_sampling_time: second [s].
    */
    WheelControl(Motor &motor, Encoder &encoder, float32_t wheel_diameter, float32_t speed_sampling_time);

    /**
     * @fn 呼び出し側のプログラムで，この関数をタイマ割り込み(任意の周期)で計測しないと動作しないので注意してください．
     */
    inline void interruptMeasureSpeed() {
        _encoder.update();
        _speed = _speed_per_pulse * static_cast<float32_t>(_encoder.getDeltaPulse());
    }

    [[nodiscard]] inline float32_t getSpeed() const { return _speed; }

    /**
     * @fn 指定の座標や距離まで連続で呼び出し続けて速度を調整する
     */
    inline void controlSpeed(float32_t speed) {
        const float32_t diff_speed = speed - _speed; // motor に印加する電圧を調整するP制御のための差分．
        _duty_ratio += diff_speed * _motor_voltage_duty;
        _motor.update(_duty_ratio);
//        _motor.update(0.08f);
    }

    void start();

    // そもそも、ここで距離も指定するのはおかしい。
//    void run(float32_t speed_mm_s, float32_t distance_mm);

    void stop();


private:

    float32_t _duty_ratio;
    float32_t _speed;  //< [mm/s] mm per second.
    Encoder &_encoder;
    Motor &_motor;
    const float32_t _speed_sampling_time; // second [s]
    const float32_t _distance_per_pulse;  // [mm/pulse] 1パルスにつき進む距離[mm]、距離計測の最低単位
    const float32_t _speed_per_pulse;     // callbackされるサンプリングタイムも考慮した値、速度計測の最低単位
    const float32_t _motor_voltage_duty;  // motorに印加する電圧の係数を組み込んだP制御パラメータ
};

}  // namespace mslh

#endif //MSLH_WHEEL_CONTROL_H
