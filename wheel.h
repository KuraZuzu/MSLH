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
#include "analog_in_dma_stream.h"

namespace mslh {

/**
 * @warning
 *     For all instances of the generated Wheel class　
 *     interruptMeasureSpeed() must be called at explicit intervals such as timer interrupt.
 *
 * Example:
 * @code
 *
 *   using namespace mslh;
 *
 * 　 // This definition is out of main().
 *   Interrupter<Wheel> wheel_interrupt;
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
 *           wheel_interrupt.run();
 *       }
 *
 *
 *   int main() {
 *       // Abbreviation Microcomputer startup settings.
 *       MX_TIM1_Init();                 //< Need setup HAL motor timer parameters.
 *       MX_TIM3_Init();                 //< Need setup HAL encoder timer parameters.
 *       MX_GPIO_Init();                 //< Need setup HAL_GPIO for motor _direction.
 *       MX_TIM6_Init();                 //< Need setup HAL ticker(callback) timer parameters.
 *
 *       Motor motor(htim1, TIM_CHANNEL_1, GPIOA,  GPIO_PIN_6, true);
 *       Encoder encoder(htim4, 500, false);
 *       AnalogInDMAStream battery(hadc1, 5);
 *       Wheel wheel(motor, encoder, battery, 13.5f, 0.01f);
 *
 *       wheel_interrupt.attach(&wheel, &Wheel::interruptControlWheel);  //< Attach function of interrupt.
 *
 *
 *       wheel.start();                                  //< Need start motor and encoder.
 *       wheel.setSpeed(500.0f, 100.0f);                 //< Running (wheel rotation) at a specified accel[mm/ss] and speed[mm/s].
 *       float32_t wheel_speed = test_wheel.getSpeed();  //< measure wheel speed
 *       wheel.stop()                                    //< Stop all work (Encoder values are retained).
 *   }
 * @endcode
 */
class Wheel {

public:
    /**
    * @note Wheel(___ , ___ , float32_t wheel_diameter, uint16_t speed_sampling_time);
    *
    * @param
    *   wheel_diameter     : milli meter [mm].
    *   speed_sampling_time: second [s].
    */
    Wheel(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery, float32_t wheel_diameter, float32_t speed_sampling_time);

    /**
     * @fn モータの速度計測&制御を行う，speed_sampling_time の間隔で実行
     * @warning この関数をタイマ割り込み(任意の周期)で計測する
     */
    inline void interruptControlWheel() {
        interruptMeasureSpeed();
        interruptTwoFreedomDegreeControl();
    }

    /**
     * @fn モータとエンコーダの動作開始．
     */
    void start();

    /**
     * @fn モータとエンコーダの動作停止．
     */
    void stop();

    /**
     * @fn モータとエンコーダ，_speedのリセット．
     */
    void reset();

    /**
     * @fn 指定した加速度と速度でモータ回転．
     * @arg (±目標加速度, ±目標速度)
     * @warning 呼び出しは1回で良い．
     */
    void setSpeed(float32_t accel, float32_t speed);

    [[gnu::warn_unused_result]] inline float32_t getSpeed() const { return _speed; }

    [[gnu::warn_unused_result]] inline float32_t getAccel() const { return _target_accel; }


private:
    /**
     * @fn 速度計測をする．speed_sampling_time の間隔で実行．
     * @warning この関数をタイマ割り込み(任意の周期)で計測する．
     */
    inline void interruptMeasureSpeed() {
        _encoder.update();
        _speed = _speed_per_pulse * static_cast<float32_t>(_encoder.getDeltaPulse());
    }


    /**
     * @fn モータの速度制御をする．speed_sampling_time の間隔で実行．
     * @note 2自由度制御(フィードフォワード & フィードバック)で構成される．
     * @warning この関数をタイマ割り込み(任意の周期)で計測する．
     *
     * なぜか、5000[mm/s]あたりで速度が頭打ちとなる．PIDのゲインが釣り合っていまうタイミングがあるのかも．
     */
    void interruptTwoFreedomDegreeControl();


    float32_t _target_accel; //< 目標加速度(指令値)
    float32_t _speed;  //< 現在の計測した実速度 [mm/s] (mm per second).
    float32_t _ideal_speed; // 理想速度(逐次、現在の測定速度に指令加速度を加算している)
    float32_t _target_speed; // 目標速度(指令値)
    float32_t _voltage; // duty比に用いる出力電圧 [0 ~ モータ供給電源電圧] [v]
    Encoder &_encoder;
    Motor &_motor;
    AnalogInDMAStream &_battery;
    const float32_t _speed_sampling_time; // second [s]
    const float32_t _distance_per_pulse;  // [mm/pulse] 1パルスにつき進む距離[mm]、距離計測の最低単位
    const float32_t _speed_per_pulse;     // callbackされるサンプリングタイムも考慮した値、速度計測の最低単位
};

}  // namespace mslh

#endif //MSLH_WHEEL_CONTROL_H
