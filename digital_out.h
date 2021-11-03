/*!
 * @file digital_out.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 *
 * @date 2021.02.14
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */


#ifndef ZUZUHALFTPPMOD1_DIGITAL_OUT_H
#define ZUZUHALFTPPMOD1_DIGITAL_OUT_H

#include "arm_math.h"
#include "gpio.h"

namespace mslh {

/**
 * @brief
 *   A digital output, used for setting the state of a GPIO.
 *
 * Example:
 * @code
 *   // Toggle a LED
 *   #include "digitalout.h"
 *
 *   using namespace mslh;
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

public:
    /**
     * @param GPIO pin in the case of "PC_5", they are ""GPIOC" and """GPIO_PIN_6".
     * */
    DigitalOut(GPIO_TypeDef *gpio_x, uint16_t gpio_pin);

    DigitalOut(const DigitalOut &obj);

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


private:
    GPIO_TypeDef *_gpio_x;
    const uint16_t _gpio_pin;
    int32_t _pinstate;
};

}  // namespace mslh

#endif //ZUZUHALFTPPMOD1_DIGITAL_OUT_H