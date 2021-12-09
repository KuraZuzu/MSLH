/*!
 * @file distance_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_DISTANCE_SENSOR_H
#define MSLH_DISTANCE_SENSOR_H

#include "arm_math.h"
#include "pwm_out.h"
#include "analog_in_dma_stream.h"
#include "digital_out.h"

namespace mslh {

/**
 * @brief
 *   This is a distance sensor class using Morinaga-type light emitting circuit.  <br>
 *   森永式発光回路を用いた距離センサクラスです．
 */
class DistanceSensor {

public:

    /**
     * @param photo_transistor is Photo-Transistor adc handler.
     */
    DistanceSensor(PWMOut led, AnalogInDMAStream photo_transistor, TIM_HandleTypeDef &sampling_htim_x);


    /**
     * @fn LEDのPWM立ち上がりをトリガーとした割り込みで呼ぶ
     * @warning PWMの"Counter Mode"を"Down"にする必要がある
     */
    inline void interruptSamplingMaxValue() {
        _get_flag = false;
        _current_value = 0;
        _previous_value = 0;
        _offset_value = _min_value;  //< オフセット値は前回の周期の物を使う
        _min_value = UINT16_MAX;
        while (_previous_value < _current_value) {
            _previous_value = _current_value;
            _current_value = _photo_transistor.read();
        }
        _get_flag = true;
    }

    /**
     * @fn _offset の値のために、任意のタイマ割り込みで呼ぶ
     */
    inline void interruptSamplingMinValue() {
        const uint16_t temp_value = _photo_transistor.read();
        if (temp_value < _min_value) _min_value = temp_value;
    }

    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read() const {
        while(!_get_flag){} //< 最新の値を持ってくるまで待つ
        return _previous_value - _offset_value; //< ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
//        return _photo_transistor.read();
    }

private:

    uint16_t getDistance_mm();

    uint16_t convert_12bit_to_mm(uint16_t value);


    PWMOut _led;
    AnalogInDMAStream _photo_transistor;
    TIM_HandleTypeDef &_sampling_htim_x;
    uint16_t _value;          //< 最終的な距離に直す値
    uint16_t _current_value;  //< _get_flag のための微分する最新の (値現在周期の値)
    uint16_t _previous_value; //< _get_flag のための微分する１つ前の値 (値現在周期の値)
    uint16_t _min_value;      //< 最低値 (値現在周期の値)
    uint16_t _offset_value;   //< 1つ前の周期の最低値を記録したもの(オフセットとして使う) (1つ前の周期の値)

    bool _get_flag;
};

}  // namespace mslh

#endif //MSLH_DISTANCE_SENSOR_H
