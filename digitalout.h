/**
 * 2021/02/14
 * @author KuraZuzu
 * */


#ifndef ZUZUHALFTPPMOD1_DIGITALOUT_H
#define ZUZUHALFTPPMOD1_DIGITALOUT_H

#include "gpio.h"

class DigitalOut {

private:
    GPIO_TypeDef* _gpio_x;
    uint16_t _gpio_pin;
    int16_t _pinstate;


public:
    DigitalOut(GPIO_TypeDef* gpio_x, uint16_t gpio_pin);

    void write(int16_t value);

    DigitalOut& operator= (int16_t value){
        _pinstate = value;
        write(_pinstate);
        return *this;
    }

    // バグの温床となる可能性もある
    operator int16_t (){
        return _pinstate;
    }

};


#endif //ZUZUHALFTPPMOD1_DIGITALOUT_H
