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

PWMOut::PWMOut(TIM_HandleTypeDef &htim_x, uint32_t channel)
        :_htim_x(htim_x)
        , _channel(channel)
        , _period(_htim_x.Init.Period)
{
}

void PWMOut::start() const {
    HAL_TIM_PWM_Start(&_htim_x, _channel);
}

void PWMOut::stop() const {
    HAL_TIM_PWM_Stop(&_htim_x, _channel);
}

void PWMOut::write(float32_t duty_ratio) {
    // The reason "+1" of last argument exists is that the duty ratio starts at 0.
    // (The output is equivalent even at 0).
    // Aが存在する理由は、Duty比が0から開始されるためです。(0でも出力は等価)
    // The reason it's not "(_period + 1)" is to avoid overflow.
    // (_period + 1) でない理由は、オーバーフローを避けるためです。
    // でも結局65535が入ってきたらオーバーフローする．
    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, duty_ratio * _period + 1);
}

void PWMOut::period(uint32_t period) {
    _period = period;
    if(!_period) __HAL_TIM_SET_COMPARE(&_htim_x, _channel, _pulse_width / _period);
}

void PWMOut::pulse_width(uint32_t pulse_width) {
    _pulse_width = pulse_width;
    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, _pulse_width / _period);
}

PWMOut &PWMOut::operator=(float32_t duty_ratio) {
    write(duty_ratio);
    return *this;
}
