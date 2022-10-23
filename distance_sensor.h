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
     * @warning P
     */
    inline void interruptSamplingMaxValue() {
        uint16_t temp_value = 0;
        _offset_value = read();
        const uint32_t tick_start = HAL_GetTick();
        while (1 <= (HAL_GetTick() - tick_start)) {  // HAL_GetTick() は1ms単位
            temp_value = _photo_transistor.read();
            if(_value < temp_value) _value = temp_value;
        }
    }

    void start();

    /**
     * @param Charge capacitor (can't set us unit).
     */
    uint16_t read() const {
        return _value - _offset_value; //< ここで一旦値を保存して getDistance_mm を呼ぶのがいいかも。
    }

    bool isOpenedLeftWall() {}
    bool isOpenedFrontWall() {}
    bool isOpenedRightWall() {}


private:

    uint16_t getDistance_mm();

    uint16_t convert_12bit_to_mm(uint16_t value);


    PWMOut _led;
    AnalogInDMAStream _photo_transistor;
    TIM_HandleTypeDef &_sampling_htim_x;
    uint16_t _value;          //< 現在のセンサー値
    uint16_t _offset_value;   //< 1つ前の周期の最低値を記録したもの(オフセットとして使う) (1つ前の周期の値)

    bool _get_flag;
};

}  // namespace mslh

#endif //MSLH_DISTANCE_SENSOR_H
