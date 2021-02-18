//
// Created by 倉澤　一詩 on 2021/02/13.
//

#ifndef ZUZUHALFTPPMOD1_MACHINE_H
#define ZUZUHALFTPPMOD1_MACHINE_H

#include "motor.h"
#include "encoder.h"
#include "buzzer.h"
#include "digitalout.h"

class Machine {

public:

    Motor* _l_motor;
    Motor* _r_motor;
    Encoder* _l_encoder;
    Encoder* _r_encoder;
    Buzzer* _buzzer;
    DigitalOut _led1, _led2, _led3;

    Machine();

    void stop();

    void move_debug();

    void buzzer_debug();

    void led_debug();

};


#endif //ZUZUHALFTPPMOD1_MACHINE_H
