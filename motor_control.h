/*!
* @file motor_control.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_MOTOR_CONTROL_H
#define ZUZUHALFTPPMOD1_MOTOR_CONTROL_H

#include "motor.h"
#include "encoder.h"

class MotorControl {

public:
    MotorControl(Motor motor, Encoder encoder)
    : _motor(motor), _encoder(encoder)
    {
    }

    void start() {
        _encoder.start();
        _encoder.reset();
        _motor.start();
        _motor.update(0);
    }

    void setSpeed(int16_t mm_per_s) {

        _encoder.update();
        if(mm_per_s == _encoder.getDeltaPulse()) {
            // どんな頻度で呼び出すんだ。分からないと。時間計測ができない。
        }
    }

    void goStraight(uint16_t distance) {
    }

private:
    Motor _motor;
    Encoder _encoder;

};


#endif //ZUZUHALFTPPMOD1_MOTOR_CONTROL_H
