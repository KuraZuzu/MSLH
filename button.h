#ifndef BUTTON_H
#define BUTTON_H

#include "arm_math.h"
#include "gpio.h"


namespace mslh {

class ButtonPullUp {

public:
    ButtonPullUp(GPIO_TypeDef* gpio_x, uint16_t gpio_pin)
    : _gpio_x(gpio_x), _gpio_pin(gpio_pin) {}

    bool read() {
        GPIO_PinState pin_state = HAL_GPIO_ReadPin(_gpio_x, _gpio_pin);

        // 10回呼んで全てHIGHであれば押されている判定（チャタリング防止）
        for (size_t i = 0; i < 10; i++) {
            pin_state = HAL_GPIO_ReadPin(_gpio_x, _gpio_pin);
            if(pin_state == GPIO_PIN_SET) return false;
            HAL_Delay(1);
        }

        return true;
    }

private:
    GPIO_TypeDef *_gpio_x;
    uint16_t _gpio_pin;
};

}

#endif // BUTTON_H