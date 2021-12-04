/*!
 * @file pwm_out.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */


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

#include "pwm_out.h"

mslh::PWMOut::PWMOut(TIM_HandleTypeDef &htim_x, uint32_t channel)
        :_htim_x(htim_x)
        , _channel(channel)
        , _period(_htim_x.Init.Period)
{
}

void mslh::PWMOut::start() const {
    HAL_TIM_PWM_Start(&_htim_x, _channel);
}

void mslh::PWMOut::stop() const {
    HAL_TIM_PWM_Stop(&_htim_x, _channel);
}


void mslh::PWMOut::period(uint32_t period) {
    _period = period;
    if(!_period) __HAL_TIM_SET_COMPARE(&_htim_x, _channel, _pulse_width / _period);
}

void mslh::PWMOut::pulse_width(uint32_t pulse_width) {
    _pulse_width = pulse_width;
    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, _pulse_width / _period);
}

mslh::PWMOut &mslh::PWMOut::operator=(float32_t duty_ratio) {
    write(duty_ratio);
    return *this;
}
