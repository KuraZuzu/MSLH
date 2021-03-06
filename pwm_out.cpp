/**
 * @date 2021/03/06
 * @author KuraZuzu
 */

#include "pwm_out.h"

PWMOut::PWMOut(TIM_HandleTypeDef &htim_x, uint32_t channel)
        :_htim_x(htim_x)
        , _channel(channel)
        , _period(_htim_x.Init.Period)
        , _pulse_width(0)
{
}

void PWMOut::start() {
    HAL_TIM_PWM_Start(&_htim_x, _channel);
}

void PWMOut::stop() {
    HAL_TIM_PWM_Stop(&_htim_x, _channel);
}

void PWMOut::write(double duty_rate) {
    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, duty_rate * _period);
}

void PWMOut::period(uint32_t period) {
    _period = period;
    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, _pulse_width / _period);
}

void PWMOut::pulse_width(uint32_t pulse_width) {
    _pulse_width = pulse_width;
    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, _pulse_width / _period);
}
