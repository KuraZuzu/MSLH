/**
 * 2019/10/23
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_MOTOR_H
#define ZUZUHALFTPPMOD1_MOTOR_H

#include "tim.h"
#include "digitalout.h"

/**
 * Turns the motor at the specified PWM rate.
 *
 * Example:
 * @code
 *
 * // The motor is rotated forward and backward at a PWM value of 0.5 (50% output) for
 * // 3 seconds each, and then stopped.
 *
 * #include "motor.h"
 *
 * Motor motor(GPIOA, GPIO_PIN_6, &htim1, TIM_CHANNEL_1, true);
 *
 * int main() {
 *
 *     MX_TIM1_Init();  // Need setup HAL encoder timer parameters.
 *     MX_GPIO_Init();  // Need setup HAL_GPIO.
 *
 *     motor.update(0.5);  // Roted forward with PWM of 50% output.
 *     HAL_Delay(3000);
 *
 *     motor.update(-0.5); // Roted backward with PWM of 50% output.
 *     HAL_Delay(3000);
 *
 *     motor.update(0);  // Stop motor.
 * }
 * @endcode
 */
class Motor {

public:

    /**
     * Motor(___ , ___ , ___ , ___ , bool cw);
     *
     * @param The cw direction corresponds to the forward rotation of your machine.
     */
    Motor(GPIO_TypeDef* phase_x, uint16_t phase_pin, TIM_HandleTypeDef* htim_x, uint32_t channel, bool cw);


    /**
     * {0.0 < duty_rate} is PWM of clock wise
     * {duty_rate < 0.0} is PWM of counter clock wise
     *
     * @param duty_rate {-1.0 ≦ duty_rate ≦ 1.0}
     */
    void update(double duty_rate);

private:
    GPIO_TypeDef* _phase_x;
    uint16_t _phase_pin;
    TIM_HandleTypeDef* _htim_x;
    uint64_t _channel;
    const GPIO_PinState _forward_wise;
};


#endif //ZUZU_HALFMOUSE_2019_MOTOR_H
