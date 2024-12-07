/*!
 * @file motor.cpp v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "motor.h"

mslh::Motor::Motor(TIM_HandleTypeDef &htim_x, uint32_t channel, GPIO_TypeDef *phase_x, uint16_t phase_pin, bool cw, MotorParams params)
        : _htim_x(htim_x)
        , _channel(channel)
        , _phase_x(phase_x)
        , _phase_pin(phase_pin)
        , _forward_wise(static_cast<GPIO_PinState>(cw))
        , _params(params)
{
}

void mslh::Motor::start() const {
    HAL_TIM_PWM_Start(&_htim_x, _channel);
}

void mslh::Motor::stop() const {
    HAL_TIM_PWM_Stop(&_htim_x, _channel);
}

void mslh::Motor::update(float32_t duty_ratio) {

    if (duty_ratio < -1.0f) duty_ratio = -1.0f;
    else if (duty_ratio > 1.0f) duty_ratio = 1.0f;

     // Set default forward wise
    HAL_GPIO_WritePin(_phase_x, _phase_pin, _forward_wise);

    // Toggle rotation wise, If duty_ratio less than 0.0
    if (duty_ratio < 0) {
        // Toggle rotation
        HAL_GPIO_TogglePin(_phase_x, _phase_pin);
        // Absolute duty_ratio.
        duty_ratio *= -1;
    }

    __HAL_TIM_SET_COMPARE(&_htim_x, _channel, (duty_ratio * _htim_x.Init.Period));
}

float32_t mslh::Motor::getResistance() { return _params.getResistance(); }

float32_t mslh::Motor::getKt() { return _params.getKt(); }

float32_t mslh::Motor::getKe() { return _params.getKe(); }
