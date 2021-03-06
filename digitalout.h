/**
 * @date 2021.02.14
 * @author Kazushi Kurasawa
 */


#ifndef ZUZUHALFTPPMOD1_DIGITALOUT_H
#define ZUZUHALFTPPMOD1_DIGITALOUT_H

#include "gpio.h"

/**
 * @brief
 *   A digital output, used for setting the state of a GPIO
 *
 * @copyright
 *   "mbed" is used as a reference. <br>
 *   (https://os.mbed.com/)
 *
 * Example:
 * @code
 *   // Toggle a LED
 *   #include "digitalout.h"
 *
 *   DigitalOut led(GPIOC, GPIO_PIN_3);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 *
 *       MX_GPIO_Init();  // Need setup HAL_GPIO.
 *
 *       while(1) {
 *           led = !led;
 *           HAL_Delay(100);
 *       }
 *   }
 * @endcode
 */
class DigitalOut {

private:
    GPIO_TypeDef* _gpio_x;
    const uint16_t _gpio_pin;
    int32_t _pinstate;


public:
    /**
     * @param GPIO pin in the case of "PC_5", they are ""GPIOC" and """GPIO_PIN_6".
     * */
    DigitalOut(GPIO_TypeDef* gpio_x, uint16_t gpio_pin);


    /**
     * @fn
     *   Rewrites GPIO_Pinstate(pin output).
     *
     * @note
     *   This class's operator supports the same functionality
     *   by treating GPIO_Pinstate(pin output) like a variable.
     *
     * @param GPIO_Pinstate. 0 is false, else true.
     */
    void write(int32_t value);


    DigitalOut& operator= (int32_t value);

    /**
     * @fn
     *   GPIO_Pinstate (pin output) can be treated like a variable.  <br>
     *   ピン出力を変数のように扱えるようになるが，バグの温床となる可能性もある．
     */
    operator int32_t () const;
//    operator int32_t (){ return _pinstate; }

};


#endif //ZUZUHALFTPPMOD1_DIGITALOUT_H
