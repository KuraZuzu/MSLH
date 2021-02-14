//
// Created by 倉澤　一詩 on 2019/10/23.
//

#ifndef ZUZUHALFTPPMOD1_MOTOR_H
#define ZUZUHALFTPPMOD1_MOTOR_H

extern "C" {
#include "tim.h"
}

class Motor {

public:
    Motor(GPIO_TypeDef* phase_x, uint16_t phase_pin, TIM_HandleTypeDef* htim_x, uint32_t channel, GPIO_PinState cw_wise);


    // 0.0 <= duty_rate <= 1.0
    void update(double duty_rate);

private:
    GPIO_PinState _forward_wise;
    TIM_HandleTypeDef* _htim_x;
    uint16_t _phase_pin;
    GPIO_TypeDef* _phase_x;
    uint64_t _channel;

};


#endif //ZUZU_HALFMOUSE_2019_MOTOR_H
