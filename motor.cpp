/**
 * @date 2019/10/23
 * @author KuraZuzu
 */

#include "motor.h"

Motor::Motor(PWMOut motor_pwm, DigitalOut motor_phase, bool cw)
        :_motor_pwm(motor_pwm)
        , _motor_phase(motor_phase)
        , _forward_wise(static_cast<GPIO_PinState>(cw)) {
}

void Motor::start() {
    _motor_pwm.start();
}

void Motor::stop() {
    _motor_pwm.stop();
}

void Motor::update(double duty_ratio) {

    if(duty_ratio < -1.0) duty_ratio = -1.0;
    else if (duty_ratio > 1.0) duty_ratio = 1.0;

    _motor_phase = _forward_wise;  //< Set default forward wise.

    // Toggle rotation wise, If duty_ratio less than 0.0.
    if (duty_ratio < 0) {
        _motor_phase = !_motor_phase;   //< Toggle rotation.
        duty_ratio *= -1;  //< Normalize duty_ratio.
    }

    _motor_pwm = duty_ratio;
}

