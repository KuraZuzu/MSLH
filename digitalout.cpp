//
// Created by 倉澤　一詩 on 2021/02/14.
//

#include "digitalout.h"

DigitalOut::DigitalOut(GPIO_TypeDef *gpio_x, uint16_t gpio_pin)
        :_gpio_x(gpio_x), _gpio_pin(gpio_pin), _pinstate(0){

    HAL_GPIO_WritePin(_gpio_x, _gpio_pin, static_cast<GPIO_PinState >(_pinstate));
}

void DigitalOut::write(int16_t value) {
    _pinstate = value;
    HAL_GPIO_WritePin(_gpio_x, _gpio_pin, static_cast<GPIO_PinState>(_pinstate));
}
