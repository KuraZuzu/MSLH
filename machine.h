/**
 * 2021/02/13
 * @author KuraZuzu
 */

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
#include "analogin_dma_stream.h"
#include <cstdio>

enum Analog{
    LEFT_FRONT,
    LEFT_SIDE,
    RIGHT_SIDE,
    RIGHT_FRONT,
    BATTERY_VOLTAGE
};

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
    AnalogInDMAStream* _analog;

    Machine();

    void stop();

    void move_debug();

    void buzzer_debug();

    void led_debug();

    void serial_debug();

    void encoder_debug();

    double battery_voltage();

    void battery_console_debug();

    void battery_warning_debug();
};

#endif //ZUZUHALFTPPMOD1_MACHINE_H
