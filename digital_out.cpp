/*!
 * @file digital_out.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.02.14
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "digital_out.h"

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
