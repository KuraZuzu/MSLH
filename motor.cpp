/**
 * 2019/10/23
 * @author KuraZuzu
 */

#include "motor.h"




void Motor::update(double duty_rate) {

    if(duty_rate < -1.0) duty_rate = -1.0;
    else if (duty_rate > 1.0) duty_rate = 1.0;

    _motor_phase = _forward_wise;

    if (duty_rate < 0) {
        _motor_phase = !_motor_phase;
        duty_rate *= -1;
    }

    _motor_pwm = duty_rate;
}

