/*!
 * @file pwm_out.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_PWM_OUT_H
#define MSLH_PWM_OUT_H

#include "arm_math.h"
#include "defines.h"
#include "tim.h"

namespace mslh {

/**
 * @brief
 *   PWM output with specified duty ratio.
 *   指定したDuty比率でPWM出力を行います。
 *
 * @copyright
 *   "mbed" is used as a reference. <br>
 *   (https://os.mbed.com/)
 *
 * Example:
 * @code
 *   // Duty比50%でPWMを出力します。
 *
 *   #include "pwm_out.h"
 *
 *   using namepsace mslh;
 *
 *   PWMOut pwm_out(htim1, TIM_CHANNEL_1);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 //
 *       MX_TIM1_Init();  //< Need setup HAL Timer parameters.
 *
 *       pwm_out.start()  //< Start PWM output.
 *
 * //      pwm_out.write(0.5);  //< Set the duty ratio with write().
 *       pwm_out = 0.5;       //< Duty ratio can also be specified in "operator =".
 *
 *       HAL_Delay(3000);
 *
 *       pwm_out.stop();  //< Stop PWM output.
 *   }
 * @endcode
 */
class PWMOut {
public:

    /**
     * @param Timer-handler and Timer-channel.
     */
    PWMOut(TIM_HandleTypeDef &htim_x, uint32_t channel);

    /**
     * @fn Start output PWM.
     */
    void start() const;


    /**
     * @fn Stop output PWM.
     */
    void stop() const;

    /**
     * @fn Write Duty ratio (0.0 ~ 1.0).
     */
    inline void write(float32_t duty_ratio) {
        // The reason "+1" of last argument exists is that the duty ratio starts at 0.
        // (The output is equivalent even at 0).
        // "+1" が存在する理由は、Duty比が0から開始されるためです。(0でも出力は等価)
        // The reason it's not "(_period + 1)" is to avoid overflow.
        // (_period + 1) でない理由は、オーバーフローを避けるためです。
        // でも結局65535が入ってきたらオーバーフローする．
        __HAL_TIM_SET_COMPARE(&_htim_x, _channel, duty_ratio * _period + 1);
    }

    void period(uint32_t period);

    void pulse_width(uint32_t pulse_width);

    /**
     * @fn
     *   Duty Ratio (duty_ratio) can be treated like a variable.  <br>
     *   Duty比(duty_ratio) を変数のように扱えるようになるが，バグの温床となる可能性もある．
     */
    PWMOut& operator = (float32_t duty_ratio);

private:
    TIM_HandleTypeDef &_htim_x;
    const uint32_t _channel;
    uint32_t &_period;
    uint32_t _pulse_width;
};

}

#endif //MSLH_PWM_OUT_H
