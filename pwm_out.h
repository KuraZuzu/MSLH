/**
 * @date 2021/03/06
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_PWM_OUT_H
#define ZUZUHALFTPPMOD1_PWM_OUT_H

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

    void write(double duty_rate);

    void period(uint32_t period);

    void pulse_width(uint32_t pulse_width);

    /**
     * @fn
     *   Duty Rate (duty_rate) can be treated like a variable.  <br>
     *   Duty比(duty_rate) を変数のように扱えるようになるが，バグの温床となる可能性もある．
     */
    PWMOut& operator = (double duty_rate);

private:
    TIM_HandleTypeDef& _htim_x;
    const uint32_t _channel;
    uint32_t& _period;
    uint32_t _pulse_width;
};


#endif //ZUZUHALFTPPMOD1_PWM_OUT_H
