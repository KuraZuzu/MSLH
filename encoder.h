/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_ENCODER_H
#define ZUZUHALFTPPMOD1_ENCODER_H

#include "tim.h"

class Encoder {

public:
    Encoder(TIM_HandleTypeDef* htim_x, bool cw_wise);

    void reset_integral();

    void start();

    void stop();

    int64_t get_delta_pulse();

    int16_t get_integral_pulse();

private:
    TIM_HandleTypeDef* _htim_x;
    const uint16_t _offset_pulse_count = 0x0FFF;
    uint16_t _pulse_count = _offset_pulse_count;
    int16_t _integral_count;
    bool _forward_wise;
};


#endif
