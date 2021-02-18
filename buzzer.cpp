//
// Created by 倉澤　一詩 on 2021/02/13.
//

#include "buzzer.h"

Buzzer::Buzzer(TIM_HandleTypeDef *htim_x, uint32_t channel): _htim_x(htim_x), _channel(channel){
    HAL_TIM_PWM_Stop(_htim_x, _channel);
}

void Buzzer::pi(){
    HAL_TIM_PWM_Start(_htim_x, _channel);
    HAL_Delay(100);
    HAL_TIM_PWM_Stop(_htim_x, _channel);
}

void Buzzer::pipi() {
    pi();
    HAL_Delay(100);
    pi();
}

void Buzzer::pipipi() {
    pi();
    HAL_Delay(100);
    pi();
    HAL_Delay(100);
    pi();
}

