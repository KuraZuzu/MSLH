/*!
 * @file motor.h v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_MOTOR_H
#define ZUZUHALFTPPMOD1_MOTOR_H

#include "pwm_out.h"
#include "tim.h"
#include "digital_out.h"
#include "defines.h"

/**
 * @brief
 *   This Class, the target of the control is Motor-Driver "DRV8836".  <br>
 *   Turns the motor at the specified PWM ratio.
 *
 * Example:
 * @code
 *   // The motor is rotated forward and backward at a PWM value of 0.5 (50% output) for
 *   // 3 seconds each, and then stopped.
 *
 *   #include "motor.h"
 *
 *   Motor motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, true);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 *
 *       MX_TIM1_Init();  //< Need setup HAL encoder timer parameters.
 *       MX_GPIO_Init();  //< Need setup HAL_GPIO.
 *
 *       motor.start();   //< Start drive motor
 *
 *       motor.update(0.5);  //< Roted forward with PWM of 50% output.
 *       HAL_Delay(3000);
 *
 *       motor.update(0.0);  //< "0.0" can be entered. In that case, the motor will stop.
 *       HAL_Delay(3000);
 *
 *       motor.update(-0.5); //< Roted backward with PWM of 50% output.
 *       HAL_Delay(3000);
 *
 *       motor.stop()  //< Stop drive motor.
 *   }
 * @endcode
 */
class Motor {

public:

    /**
     * @note
     *   Motor(___ , ___ , ___, ___,  bool cw);  <br>
     *
     * @param
     *   bool cw: The direction corresponds
     *   to the forward rotation of your machine.
     */
    Motor(TIM_HandleTypeDef &htim_x, uint32_t channel, GPIO_TypeDef *phase_x, uint16_t phase_pin, bool cw);


    /**
     * @fn Copy constructor.
     */
    Motor(const Motor &obj);


    virtual /**
     * @fn Start motor.
     */
    void start();

    /**
     * @fn Stop motor.
     */
    void stop();

    /**
     * @fn Specifies the PWM of the motor.
     *
     * @details
     *   {0.0 < duty_ratio} is PWM of clock wise  <br>
     *   {duty_ratio < 0.0} is PWM of counter clock wise
     *
     * @param duty_ratio {-1.0 ≦ duty_ratio ≦ 1.0}
     */
    void update(float32_t duty_ratio);

private:
    GPIO_TypeDef *_phase_x;
    const uint16_t _phase_pin;
    TIM_HandleTypeDef &_htim_x;
    const uint64_t _channel;
    const GPIO_PinState _forward_wise;
};

#endif //ZUZU_HALFMOUSE_2019_MOTOR_H
