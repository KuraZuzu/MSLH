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

    int32_t get_delta_pulse();

    int64_t get_total_pulse();

    /** 現在の integral_pulse を代入します．
     *  delta_pules では無い点に注意してください ．
     *  基本的には get_delta_pulse() の最後に呼び出します．*/
    void update_rotation_count(int integral_pulse){
        _rotation_count = integral_pulse / _one_rotation_pulse;
        _integral_count = integral_pulse % _one_rotation_pulse;
    }

private:
    TIM_HandleTypeDef* _htim_x;
    const uint16_t _offset_pulse_count = 0x0FFF;
    uint16_t _pulse_count;
    int64_t _integral_count;
    bool _forward_wise;
    uint16_t _one_rotation_pulse;
    int64_t _rotation_count;
};


#endif
