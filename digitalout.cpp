//
// Created by 倉澤　一詩 on 2021/02/14.
//

#include "digitalout.h"

DigitalOut::DigitalOut(GPIO_TypeDef *gpio_x, uint16_t gpio_pin)
        :_gpio_x(gpio_x), _gpio_pin(gpio_pin){

    HAL_GPIO_WritePin(_gpio_x, _gpio_pin, GPIO_PIN_RESET);
}

void DigitalOut::write(int16_t value) {
    if(!value) HAL_GPIO_WritePin(_gpio_x, _gpio_pin, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(_gpio_x, _gpio_pin, GPIO_PIN_SET);
}

DigitalOut &DigitalOut::operator=(int16_t value) {
    write(value);
    return *this;
}

DigitalOut &DigitalOut::operator!() {
    HAL_GPIO_TogglePin(_gpio_x, _gpio_pin);
    return *this;
}
