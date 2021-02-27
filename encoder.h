/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_ENCODER_H
#define ZUZUHALFTPPMOD1_ENCODER_H

#include "tim.h"

class Encoder {

public:
    Encoder(TIM_HandleTypeDef* htim_x, uint16_t one_rotation_pulse, bool cw_wise);

    void start();

    void stop();

    void reset_integral();

    int64_t get_delta_pulse();

    int64_t get_rotation_count();

    int64_t get_total_pulse();


private:

    void update_encoder();

    void update_delta_pulse();

    void update_rotation_count();

    TIM_HandleTypeDef* _htim_x;
    const uint16_t _offset_pulse = 0x0FFF;
    uint16_t _pulse_count;
    int64_t _integral_pulse;
    bool _forward_wise;
    uint16_t _one_rotation_pulse;
    int64_t _rotation_count;
    int64_t _delta_pulse;
};


#endif
