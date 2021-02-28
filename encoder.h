/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_ENCODER_H
#define ZUZUHALFTPPMOD1_ENCODER_H

#include "tim.h"

class Encoder {

public:
    Encoder(TIM_HandleTypeDef* htim_x, uint16_t one_rotation_pulse, bool cw);

    void start();

    void stop();

    void reset();

    /** 主に以下の getter を用いる．
     *  get_delta_pulse(), get_rotation_count(), get_surplus_pulse */
    int32_t get_delta_pulse();    // 前回とのパルス差分を取得
    int64_t get_rotation_count(); // 現在の回転数を取得
    int64_t get_surplus_pulse();  //１回転に満たない余剰パルスを取得


    /** 以下の関数は主にデバッグ用．
     *  get_integral_pulse() */
    int64_t get_total_pulse(); // 蓄積パルスが多すぎるとオーバーフローする恐れあり


private:

    void update_encoder();

    void update_pulse();

    void update_rotation_count();


    TIM_HandleTypeDef* _htim_x;
    uint16_t _one_rotation_pulse;
    bool _forward_wise;

    /** エンコーダ内部のカウントは uint16_t の容量でカウントされる．
     *  　　0 ≦ _htim_x->Instance->CNT ≦ 65535
     *  (uint16_t なので 65535 が最大の設定値，.ioc から設定可能)
     *
     *  なので，パルス差分カウントのためのオフセットは中間の 0x0FFF */
    const uint16_t _offset_pulse ; // 0x0FFF
    int32_t _delta_pulse;
    int64_t _integral_pulse;
    int64_t _rotation_count;
};


#endif
