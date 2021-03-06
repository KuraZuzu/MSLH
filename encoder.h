/**
 * @date 2019/10/23
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_ENCODER_H
#define ZUZUHALFTPPMOD1_ENCODER_H

#include "tim.h"

/**
 * @brief
 *   Incremental-rotary-Encoder using the HAL driver's encoder function.
 *
 * @note
 *   Returns the rotation speed and pulses based on the AB phase of the incremental encoder,
 *   without considering the Z phase.
 *
 * Example:
 * @code
 *   // Get the number of encoder rotations and delta pulses.
 *   // If Encoder one-rotation pulse of A or B is 500.
 *
 *   #include "encoder.h"
 *
 *   Encoder encoder(htim3, 500, false);
 *
 *   int main() {
 *       MX_TIM3_Init();  //< Need setup HAL encoder timer parameters.
 *       MX_GPIO_Init();  //< Need setup HAL_GPIO.
 *
 *       // [ STEP 0 ]
 *       int64_t delta_pulse;
 *       int64_t rotation_count;
 *
 *       // Start encoder measurement
 *       encoder.start();
 *
 *
 *       // [ STEP 1 ]
 *       encoder.update();
 *       delta_pulse = encoder.get_delta_pulse();        //< delta_pulse between step 0 and 1
 *       rotation_count = encoder.get_rotation_count();  //< rotation_count at step 1
 *
 *       // *** Need encoder.update(); , if you get current data. ***
 *       delta_pulse = encoder.get_delta_pulse();        //< delta_pulse between step 0 and 1
 *       rotation_count = encoder.get_rotation_count();  //< rotation_count at step 1
 *
 *
 *       // [ STEP 2 ]
 *       encoder.update();
 *       delta_pulse = encoder.get_delta_pulse();        //< delta_pulse between step 1 and 2
 *       rotation_count = encoder.get_rotation_count();  //< rotation_count at step 2
 *   }
 * @endcode
 */
class Encoder {

public:

    /**
     * @note Encoder(___ , ___ , ___ , bool cw);
     *
     * @param The cw direction corresponds to the forward rotation of your machine.
     */
    Encoder(TIM_HandleTypeDef& htim_x, uint16_t one_rotation_pulse, bool cw);

    /**
     * @fn Start encoder measurement.
     */
    void start();

    /**
     * @fn Stop encoder measurement.
     */
    void stop();

    /**
     * @fn Reset all measured data.
     */
    void reset();

    /**
     *  @fn
     *    Update the number of pulses and rotation speed of the encoder.  <br>
     *    エンコーダのパルス数と回転数を更新します．
     *
     *  @details
     *    Must be called just before using getter of this class. <br>
     *    このクラスの getter を呼び出す直前に呼び出す必要があります．
     *
     *  @param Updates pulses and rotations.
     */
    void update();


     /**
      *  @return
      *    Pulse difference between the latest call to update_encoder()
      *    and the last call to update()
      */
     int32_t get_delta_pulse();    // 最新で呼んだ update_encoder() と前回呼んだ update() 時点でのパルス差分を取得


    /**
     *  @return
     *    Total number of rotations at the time of the latest update() call.
     */
    int64_t get_rotation_count(); //< 最新で呼んだ update() 時点での合計回転数を取得


    /**
     * @return
     *   Excess pulses of less than one revolution
     *   at the time of the latest call to update().
     */
    int64_t get_surplus_pulse();  //< 最新で呼んだ update() 時点での１回転未満の余剰パルスを取得


    /**
     * @fn
     *   It is function for debug. Do not use it.  <br>
     *   この関数は主にデバッグ用です．使わないでください．
     *
     * @return Total number of pulses counted so far.
     */
    int64_t get_total_pulse(); //< 蓄積パルスが多すぎるとオーバーフローする恐れあり



private:

    TIM_HandleTypeDef& _htim_x;
    const uint16_t _one_rotation_pulse;
    const bool _forward_wise;


    /**
     * @note
     *   エンコーダ内部のカウントは uint16_t の範囲内でカウントされる．  <br>
     *  　　  0 ≦ _htim_x->Instance->CNT ≦ 65535      <br>
     *    (最大値は 0~65535 の範囲で，.ioc から設定可能)  <br>
     *
     *    パルス差分カウントのためのオフセットは中間の 0x0FFF=(65536/2 - 1) で初期化される．
     */
    const uint16_t _offset_pulse ; //< 0x0FFF

    int32_t _delta_pulse;
    int64_t _integral_pulse;
    int64_t _rotation_count;

    void update_pulse();

    void update_rotation_count();
};


#endif
