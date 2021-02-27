/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_MOTOR_H
#define ZUZUHALFTPPMOD1_MOTOR_H

#include "tim.h"
#include "digitalout.h"


class Motor {

public:
    /** bool cw is forward wise of your machine */
    Motor(GPIO_TypeDef* phase_x, uint16_t phase_pin, TIM_HandleTypeDef* htim_x, uint32_t channel, bool cw);

    void update(double duty_rate);

private:
    GPIO_TypeDef* _phase_x;
    uint16_t _phase_pin;
    TIM_HandleTypeDef* _htim_x;
    uint64_t _channel;
    GPIO_PinState _forward_wise;
};


#endif //ZUZU_HALFMOUSE_2019_MOTOR_H
