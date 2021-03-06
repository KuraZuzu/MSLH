/**
 * @date 2019/10/23
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_MOTOR_H
#define ZUZUHALFTPPMOD1_MOTOR_H

#include "pwm_out.h"
#include "tim.h"
#include "digitalout.h"

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
 *   Motor motor(PWMOut(htim1, TIM_CHANNEL_1), DigitalOut(GPIOA, GPIO_PIN_6), true);
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
     *   Motor(___ , ___ , bool cw);  <br>
     *
     * @param
     *   bool cw: The direction corresponds
     *   to the forward rotation of your machine.
     */
    Motor(PWMOut motor_pwm, DigitalOut motor_phase, bool cw);


    /**
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
    void update(double duty_ratio);

private:
    PWMOut _motor_pwm;
    DigitalOut _motor_phase;
    const GPIO_PinState _forward_wise;
};


#endif //ZUZU_HALFMOUSE_2019_MOTOR_H
