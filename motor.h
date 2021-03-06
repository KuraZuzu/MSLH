/**
 * 2019/10/23
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
 *   Turns the motor at the specified PWM rate.
 *
 * Example:
 * @code
 *   // The motor is rotated forward and backward at a PWM value of 0.5 (50% output) for
 *   // 3 seconds each, and then stopped.
 *
 *   #include "motor.h"
 *
 *   Motor motor(GPIOA, GPIO_PIN_6, htim1, TIM_CHANNEL_1, true);
 *
 *   int main() {
 *
 *       MX_TIM1_Init();  //< Need setup HAL encoder timer parameters.
 *       MX_GPIO_Init();  //< Need setup HAL_GPIO.
 *
 *       motor.update(0.5);  //< Roted forward with PWM of 50% output.
 *       HAL_Delay(3000);
 *
 *       motor.update(-0.5); //< Roted backward with PWM of 50% output.
 *       HAL_Delay(3000);
 *
 *       motor.update(0.0);  //< Stop motor.
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
     *   The (bool cw) direction corresponds
     *   to the forward rotation of your machine.
     */
    Motor(PWMOut motor_pwm, DigitalOut motor_phase, bool cw)
    :_motor_pwm(motor_pwm)
    , _motor_phase(motor_phase)
    , _forward_wise(static_cast<GPIO_PinState>(cw)) {
    }

    void start() {
        _motor_pwm.start();
    }

    void stop() {
        _motor_pwm.stop();
    }

    /**
     * @fn Specifies the PWM of the motor.
     *
     * @details
     *   {0.0 < duty_rate} is PWM of clock wise  <br>
     *   {duty_rate < 0.0} is PWM of counter clock wise
     *
     * @param duty_rate {-1.0 ≦ duty_rate ≦ 1.0}
     */
    void update(double duty_rate);

private:
    PWMOut _motor_pwm;
    DigitalOut _motor_phase;
    const GPIO_PinState _forward_wise;
};


#endif //ZUZU_HALFMOUSE_2019_MOTOR_H
