//
// Created by 倉澤　一詩 on 2021/02/14.
//

#ifndef ZUZUHALFTPPMOD1_DIGITALOUT_H
#define ZUZUHALFTPPMOD1_DIGITALOUT_H

#include "gpio.h"

class DigitalOut {

private:
    GPIO_TypeDef* _gpio_x;
    uint16_t _gpio_pin;


public:
    DigitalOut(GPIO_TypeDef* gpio_x, uint16_t gpio_pin);

    void write(int16_t value);

    DigitalOut& operator= (int16_t value);

    DigitalOut& operator! ();

};


#endif //ZUZUHALFTPPMOD1_DIGITALOUT_H
