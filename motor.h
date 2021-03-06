/*!
 * @file motor.h v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_PWM_OUT_H
#define ZUZUHALFTPPMOD1_PWM_OUT_H

/**
 * @brief
 *   PWM output with specified duty ratio.
 *   指定したDuty比率でPWM出力を行います。
 *
 * @copyright
 *   "mbed" is used as a reference. <br>
 *   (https://os.mbed.com/)
 *
 * Example:
 * @code
 *   // Duty比50%でPWMを出力します。
 *
 *   #include "pwm_out.h"
 *
 *   PWMOut pwm_out(htim1, TIM_CHANNEL_1);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 //
 *       MX_TIM1_Init();  //< Need setup HAL Timer parameters.
 *
 *       pwm_out.start()  //< Start PWM output.
 *
 * //      pwm_out.write(0.5);  //< Set the duty ratio with write().
 *       pwm_out = 0.5;       //< Duty ratio can also be specified in "operator =".
 *
 *       HAL_Delay(3000);
 *
 *       pwm_out.stop();  //< Stop PWM output.
 *   }
 * @endcode
 */

#include "tim.h"

class PWMOut {
public:
    /**
     * @param Timer-handler and Timer-channel.
     */
    PWMOut(TIM_HandleTypeDef& htim_x, uint32_t channel);

    /**
     * @fn Start output PWM.
     */
    void start();


    /**
     * @fn Stop output PWM.
     */
    void stop();

    void write(double duty_ratio);

    void period(uint32_t period);

    void pulse_width(uint32_t pulse_width);

    /**
     * @fn
     *   Duty Ratio (duty_ratio) can be treated like a variable.  <br>
     *   Duty比(duty_ratio) を変数のように扱えるようになるが，バグの温床となる可能性もある．
     */
    PWMOut& operator = (double duty_ratio);

private:
    TIM_HandleTypeDef& _htim_x;
    const uint32_t _channel;
    uint32_t& _period;
    uint32_t _pulse_width;
};


#endif //ZUZUHALFTPPMOD1_PWM_OUT_H

#ifndef ZUZUHALFTPPMOD1_MOTOR_H
#define ZUZUHALFTPPMOD1_MOTOR_H

#include "pwm_out.h"
#include "tim.h"
#include "digital_out.h"

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
