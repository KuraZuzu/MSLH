/*!
 * @file motor.cpp v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "motor.h"

Motor::Motor(TIM_HandleTypeDef &htim_x, uint32_t channel, GPIO_TypeDef *phase_x, uint16_t phase_pin, bool cw)
        : _phase_x(phase_x),
          _phase_pin(phase_pin),
          _htim_x(htim_x),
          _channel(channel),
          _forward_wise(static_cast<GPIO_PinState>(cw))
{
}

void Motor::start() {
    HAL_TIM_PWM_Start(&_htim_x, _channel);
}

void Motor::stop() {
    HAL_TIM_PWM_Stop(&_htim_x, _channel);
}

void Motor::update(float32_t duty_ratio) {

    if(duty_ratio < -1.0f) duty_ratio = -1.0f;
    else if (duty_ratio > 1.0f) duty_ratio = 1.0f;

    HAL_GPIO_WritePin(_phase_x, _phase_pin, _forward_wise);  //< Set default forward wise.

    // Toggle rotation wise, If duty_ratio less than 0.0.
    if (duty_ratio < 0) {
        HAL_GPIO_TogglePin(_phase_x, _phase_pin);   //< Toggle rotation.
        duty_ratio *= -1;  //< Normalize duty_ratio.
    }

    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, (duty_ratio * _htim_x.Init.Period));
}

