/**
 * 2021/02/14
 * @author KuraZuzu
 */

#include "digitalout.h"

DigitalOut::DigitalOut(GPIO_TypeDef *gpio_x, uint16_t gpio_pin)
        :_gpio_x(gpio_x), _gpio_pin(gpio_pin), _pinstate(0){

    HAL_GPIO_WritePin(_gpio_x, _gpio_pin, static_cast<GPIO_PinState >(_pinstate));
}

void DigitalOut::write(int32_t value) {
    _pinstate = value;
    HAL_GPIO_WritePin(_gpio_x, _gpio_pin, static_cast<GPIO_PinState>(_pinstate));
}

DigitalOut &DigitalOut::operator=(int32_t value) {
    _pinstate = value;
    write(_pinstate);
    return *this;
}

DigitalOut::operator int32_t() const { return _pinstate; }
