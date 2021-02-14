//
// Created by 倉澤　一詩 on 2021/02/13.
//

#include "machine.h"

Machine::Machine()
:_led1(GPIOC, GPIO_PIN_3), _led2(GPIOC, GPIO_PIN_4), _led3(GPIOC, GPIO_PIN_5){
    _l_motor = new Motor(GPIOA, GPIO_PIN_6, &htim1, TIM_CHANNEL_1, static_cast<GPIO_PinState >(1));
    _r_motor = new Motor(GPIOA, GPIO_PIN_7, &htim1, TIM_CHANNEL_2, static_cast<GPIO_PinState >(0));
    _l_encoder = new Encoder(&htim4, true);
    _r_encoder = new Encoder(&htim3, false);
    _buzzer = new Buzzer(&htim8, TIM_CHANNEL_1);

    stop();
}

void Machine::move_debug() {
    _l_motor->update(0.8);
    _r_motor->update(0.8);
}

void Machine::stop() {
    _l_motor->update(0);
    _r_motor->update(0);
}

void Machine::buzzer_debug() {
    _buzzer->pi();
    HAL_Delay(1000);
    _buzzer->pipi();
    HAL_Delay(1000);
    _buzzer->pipipi();
}

void Machine::led_debug() {
    while (1) {
        _led1 = !_led1;
        _led2 = !_led2;
        _led3 = !_led3;
        HAL_Delay(500);
    }
//    _led1.write(1);
//    _led2.write(1);
//    _led3.write(1);

}
