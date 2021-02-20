/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#include "motor.h"

/** (bool cw) is forward wise of your machine */
Motor::Motor(GPIO_TypeDef* phase_x, uint16_t phase_pin, TIM_HandleTypeDef* htim_x, uint32_t channel, bool cw)
: _phase_x(phase_x),
_phase_pin(phase_pin),
_htim_x(htim_x),
_channel(channel),
_forward_wise(static_cast<GPIO_PinState>(cw))
{
//    MX_TIM1_Init();
//    MX_GPIO_Init();
//    _phase_x = phase_x;
//    _phase_pin = phase_pin;
//    _htim_x = htim_x;
//    _channel = channel;
//    _forward_wise = static_cast<GPIO_PinState>(cw);
    HAL_GPIO_WritePin(_phase_x, _phase_pin, _forward_wise);
    HAL_TIM_PWM_Start(_htim_x, _channel);
    update(0.0);
}


/** -1.0 ≦ duty_rate ≦ 1.0 /
 *
 * {0.0 < duty_rate} is PWM of clock wise
 * {duty_rate < 0.0} is PWM of counter clock wise
 *
 * @param duty_rate
 */
void Motor::update(double duty_rate) {

    if(duty_rate < -1.0) duty_rate = -1.0;
    else if (duty_rate > 1.0) duty_rate = 1.0;

    HAL_GPIO_WritePin(_phase_x, _phase_pin, _forward_wise);

    if (duty_rate < 0) {
        HAL_GPIO_TogglePin(_phase_x, _phase_pin);
        duty_rate *= -1;
    }
    //もう少し計算量を少なくできそう．
    //TgglePin を使わないとか

    __HAL_TIM_SET_COMPARE(_htim_x, _channel, (duty_rate * _htim_x->Init.Period));
}

