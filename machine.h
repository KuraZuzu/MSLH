/**
 * 2021/02/13
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_MACHINE_H
#define ZUZUHALFTPPMOD1_MACHINE_H

#include "motor.h"
#include "encoder.h"
#include "buzzer.h"
#include "digitalout.h"
#include "usart.h"
#include "adc.h"
#include "spi.h"
#include "dma.h"
#include "serial_utility.h"

class Machine {

public:
    DigitalOut _led1;
    DigitalOut _led2;
    DigitalOut _led3;
    Motor* _l_motor;
    Motor* _r_motor;
    Encoder* _l_encoder;
    Encoder* _r_encoder;
    Buzzer* _buzzer;
    SerialUtility* _usart;

    Machine();

    void stop();

    void move_debug();

    void buzzer_debug();

    void led_debug();

    void print_debug();
};


#endif //ZUZUHALFTPPMOD1_MACHINE_H
