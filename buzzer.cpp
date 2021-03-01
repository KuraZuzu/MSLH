/**
 * 2021/02/13
 * @author KuraZuzu
 */
#include "buzzer.h"

Buzzer::Buzzer(TIM_HandleTypeDef *htim_x, uint32_t channel)
: _htim_x(htim_x), _channel(channel){
    HAL_TIM_PWM_Stop(_htim_x, _channel);
}

void Buzzer::beep_x(uint16_t times) {

    for(uint16_t i = 0; i < times; i++) {
        HAL_TIM_PWM_Start(_htim_x, _channel);
        HAL_Delay(50);
        HAL_TIM_PWM_Stop(_htim_x, _channel);
        HAL_Delay(50);
    }
}