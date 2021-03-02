/**
 * 2021/02/14
 * @author KuraZuzu
 */


#ifndef ZUZUHALFTPPMOD1_DIGITALOUT_H
#define ZUZUHALFTPPMOD1_DIGITALOUT_H

#include "gpio.h"

/**
 * I used "mbed" as a reference.
 * A digital output, used for setting the state of a GPIO
 *
 * Example:
 * @code
 * // Toggle a LED
 * #include "digitalout.h"
 *
 * DigitalOut led(GPIOC, GPIO_PIN_3);
 *
 * int main() {
 *
 *     MX_GPIO_Init();  // Need setup HAL_GPIO.
 *
 *     while(1) {
 *         led = !led;
 *         HAL_Delay(100);
 *     }
 * }
 * @endcode
 */
class DigitalOut {

private:
    GPIO_TypeDef* _gpio_x;
    const uint16_t _gpio_pin;
    int16_t _pinstate;


public:
    DigitalOut(GPIO_TypeDef* gpio_x, uint16_t gpio_pin);

    void write(int16_t value);

    DigitalOut& operator= (int16_t value){
        _pinstate = value;
        write(_pinstate);
        return *this;
    }

    // ピン出力のON/OFFを変数のように扱えるようになるが，
    // バグの温床となる可能性もある
    operator int16_t (){ return _pinstate; }

};


#endif //ZUZUHALFTPPMOD1_DIGITALOUT_H
