/**
 * @date 2021.03.06
 * @author Kazushi Kurasawa
 */

#include "pwm_out.h"

PWMOut::PWMOut(TIM_HandleTypeDef& htim_x, uint32_t channel)
        :_htim_x(htim_x)
        , _channel(channel)
        , _period(_htim_x.Init.Period)
{
}

void PWMOut::start() {
    HAL_TIM_PWM_Start(&_htim_x, _channel);
}

void PWMOut::stop() {
    HAL_TIM_PWM_Stop(&_htim_x, _channel);
}

void PWMOut::write(double duty_ratio) {
    // The reason "+1" of last argument exists is that the duty ratio starts at 0.
    // (The output is equivalent even at 0).
    // Aが存在する理由は、Duty比が0から開始されるためです。(0でも出力は等価)
    // The reason it's not "(_period + 1)" is to avoid overflow.
    // (_period + 1) でない理由は、オーバーフローを避けるためです。
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

PWMOut &PWMOut::operator=(double duty_ratio) {
    write(duty_ratio);
    return *this;
}
