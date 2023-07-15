/*!
 * @file encoder.h v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.10.23
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_ENCODER_H
#define MSLH_ENCODER_H

#include "arm_math.h"
#include "tim.h"

namespace mslh {

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
 *   using namespace mslh;
 *
 *   Encoder encoder(htim3, 500, false);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 *
 *       MX_TIM3_Init();  //< Need setup HAL encoder timer parameters.
 *       MX_GPIO_Init();  //< Need setup HAL_GPIO.
 *
 *       // [ STEP 0 ]
 *       int64_t delta_pulse;
 *       int64_t rotation_count;
 *
 *       encoder.start();  //< Start encoder measurement.
 *
 *
 *
 *       // [ STEP 1 ]
 *       encoder.update();
 *       delta_pulse = encoder.getDeltaPulse();        //< delta_pulse between step 0 and 1
 *       rotation_count = encoder.getRotationCount();  //< rotation_count at step 1
 *
 *       // *** Need encoder.update(); , if you get current data. ***
 *       delta_pulse = encoder.getDeltaPulse();        //< delta_pulse between step 0 and 1
 *       rotation_count = encoder.getRotationCount();  //< rotation_count at step 1
 *
 *
 *       // [ STEP 2 ]
 *       encoder.update();
 *       delta_pulse = encoder.getDeltaPulse();        //< delta_pulse between step 1 and 2
 *       rotation_count = encoder.getRotationCount();  //< rotation_count at step 2
 *   }
 * @endcode
 */
class Encoder {

public:

    /**
     * @note Encoder(___ , ___ , ___ , bool cw);
     *
     * @param The cw _direction corresponds to the forward rotation of your machine.
     */
    Encoder(TIM_HandleTypeDef &htim_x, int32_t  one_rotation_pulse, bool cw);


    /**
     * @fn Start encoder measurement.
     */
    void start() const ;

    /**
     * @fn Stop encoder measurement.
     */
    void stop() const ;

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
    [[gnu::warn_unused_result]] inline int32_t getDeltaPulse() const { return _delta_pulse; }  //< 最新で呼んだ update_encoder() と前回呼んだ update() 時点でのパルス差分を取得


    /**
     * @warning 返り値は 32bit のため 2147483647[mm]=2147[km] まで計算可能 ．足りなければ64bitに変更．
     * @return Total number of pulses counted so far.
     */
    [[gnu::warn_unused_result]] inline int32_t getTotalPulse() const { return _total_pulse; }


    /**
     *  @return
     *    Total number of rotations at the abs_time of the latest update() call.
     */
    [[gnu::warn_unused_result]] inline int32_t getRotationCount() const { return _total_pulse / _one_rotation_pulse; } //< 最新で呼んだ update() 時点での合計回転数を取得


    /**
     * @return
     *   Excess pulses of less than one revolution
     *   at the abs_time of the latest call to update().
     */
    [[gnu::warn_unused_result]] inline int32_t getSurplusPulse() const { return _total_pulse % _one_rotation_pulse; }  //< 最新で呼んだ update() 時点での１回転未満の余剰パルスを取得


    /**
     * @return
     *   Returns a pulse for one rotation of the encoder.
     */
    [[gnu::warn_unused_result]] inline int32_t getOneRotationPulse() const { return _one_rotation_pulse; }

private:

    int32_t _delta_pulse;
    int32_t _total_pulse;
    TIM_HandleTypeDef& _htim_x;
    const bool _forward_wise;
    const int32_t _one_rotation_pulse;

    /**
     * @note
     *   エンコーダ内部のカウントは uint16_t の範囲内でカウントされる．(ただし型は uint32_t)  <br>
     *  　　  0 ≦ _htim_x->Instance->CNT ≦ 65535      <br>
     *    (最大値は 0~65535 の範囲で，.ioc から設定可能)  <br>
     *
     *    パルス差分カウントのためのオフセットは中間の 0x0FFF=(65536/2 - 1) で初期化する．
     */
    const uint32_t _offset_pulse ; //< 0x0FFF
};

}  // namespace mslh

#endif //MSLH_ENCODER_H
