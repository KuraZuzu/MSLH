/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_ENCODER_H
#define ZUZUHALFTPPMOD1_ENCODER_H

#include "tim.h"


/** Incremental-rotary-Encoder using the HAL driver's encoder function.
 *
 * Returns the rotation speed and pulses based on the AB phase of the incremental encoder,
 * without considering the Z phase.
 *
 * Example:
 * @code
 * // Get the number of encoder rotations and delta pulses.
 *
 * #include "encoder.h"
 *
 * Encoder encoder(&htim3, 500*4, false);
 *
 * int main() {
 *
 *    // *** Need setup HAL encoder timer parameters. ***
 *    MX_TIM3_Init();
 *
 *    // [ STEP 0 ]
 *    int64_t delta_pulse;
 *    int64_t rotation_count;
 *    encoder.start(); // Init encoder setup.*
 *
 *
 *    // [ STEP 1 ]
 *    encoder.update();
 *    delta_pulse = encoder.get_delta_pulse();       // delta_pulse between step 0 and 1
 *    rotation_count = encoder.get_rotation_count(); // rotation count when step 1*
 *
 *    // *** Need encoder.update(); , if you get current data. ***
 *    delta_pulse = encoder.get_delta_pulse();       // delta_pulse between step 0 and 1
 *    rotation_count = encoder.get_rotation_count(); // rotation count when step 1
 *
 *
 *    // [ STEP 2 ]
 *    encoder.update();
 *    delta_pulse = encoder.get_delta_pulse();       // delta_pulse between step 1 and 2
 *    rotation_count = encoder.get_rotation_count(); // rotation count when step 2
 * }
 * @endcode
 */
class Encoder {

public:
    Encoder(TIM_HandleTypeDef* htim_x, uint16_t one_rotation_pulse, bool cw);

    void start();

    void stop();

    void reset();

    /** エンコーダのパルス数と回転数を更新する．
     *  getter を用いる前に呼び出す必要がある．*/
    void update();


    /** 主に以下の getter を用いる．
     *  get_delta_pulse(), get_rotation_count(), get_surplus_pulse */
    int32_t get_delta_pulse();    // 最新で呼んだ update_encoder() と前回呼んだ update() 時点でのパルス差分を取得
    int64_t get_rotation_count(); // 最新で呼んだ update() 時点での回転数を取得
    int64_t get_surplus_pulse();  // 最新で呼んだ update() 時点で，１回転に満たない余剰パルスを取得


    /** 以下の関数は主にデバッグ用．
     *  get_integral_pulse() */
    int64_t get_total_pulse(); // 蓄積パルスが多すぎるとオーバーフローする恐れあり



private:

    TIM_HandleTypeDef* _htim_x;
    const uint16_t _one_rotation_pulse;
    const bool _forward_wise;


    /** エンコーダ内部のカウントは uint16_t の範囲内でカウントされる．
     *  　　0 ≦ _htim_x->Instance->CNT ≦ 65535
     *  (uint16_t なので 65535 が最大の設定値，.ioc から設定可能)
     *
     *  なので，パルス差分カウントのためのオフセットは中間の 0x0FFF=(65536/2 - 1) で初期化される*/
    const uint16_t _offset_pulse ; // 0x0FFF

    int32_t _delta_pulse;
    int64_t _integral_pulse;
    int64_t _rotation_count;

    void update_pulse();

    void update_rotation_count();

};


#endif
