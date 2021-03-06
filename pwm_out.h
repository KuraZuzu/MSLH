/**
 * @date 2021/03/06
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_PWM_OUT_H
#define ZUZUHALFTPPMOD1_PWM_OUT_H

#include "tim.h"

class PWMOut {
public:
    PWMOut(TIM_HandleTypeDef& htim_x, uint32_t channel);

    void start();

    void stop();

    void write(double duty_rate);

    void period(uint32_t period);

    void pulse_width(uint32_t pulse_width);


    PWMOut& operator = (double duty_rate) {
        write(duty_rate);
        return *this;
    }

private:
    TIM_HandleTypeDef& _htim_x;
    const uint32_t _channel;
    uint32_t _period;
    uint32_t _pulse_width;
};


#endif //ZUZUHALFTPPMOD1_PWM_OUT_H
