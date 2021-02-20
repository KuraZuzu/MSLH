/**
 * 2021/02/13
 * @author KuraZuzu
 * */
#include "buzzer.h"

Buzzer::Buzzer(TIM_HandleTypeDef *htim_x, uint32_t channel): _htim_x(htim_x), _channel(channel){
    /** 最初に MX_TIM8_Init() で設定しないと動作しない*/
//    MX_TIM8_Init();
    HAL_TIM_PWM_Stop(_htim_x, _channel);
//    HAL_TIM_PWM_Start(_htim_x, _channel);
}

void Buzzer::pi(){
    HAL_TIM_PWM_Start(_htim_x, _channel);
    HAL_Delay(50);
    HAL_TIM_PWM_Stop(_htim_x, _channel);
}

void Buzzer::pipi() {
    pi();
    HAL_Delay(50);
    pi();
}

void Buzzer::pipipi() {
    pi();
    HAL_Delay(50);
    pi();
    HAL_Delay(50);
    pi();
}

