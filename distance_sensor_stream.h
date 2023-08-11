/*!
 * @file analogin_dma_stream.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2023.08.10
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */


#ifndef MSLH_DISTANCE_SENSOR_STREAM_H
#define MSLH_DISTANCE_SENSOR_STREAM_H

#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include "arm_math.h"
#include "adc.h"
#include "digital_out.h"


namespace mslh {

class DistanceSensor {

public:
    DistanceSensor(DigitalOut led): _led(led) {}

    inline const uint16_t read() {
        return _adc_value;
    }

    inline void write(uint16_t adc_value) {
        _adc_value = adc_value;
    }

private:
    DigitalOut _led;
    uint16_t _adc_value;
    friend class DistanceSensorStream;
};

/**
* @warning
*   DistanceSensorのrankは以下のように設定してください
*   ・１つのチャネルのセンサのみを引数に与えてください
*   ・１つのセンサにつきrank2つぶんを連続で設定してください
*   ・センサ１、センサ２、センサ３を設定する場合
*/
template<typename... DistanceSensorArgs>
class DistanceSensorStream {

public:

    DistanceSensorStream(ADC_HandleTypeDef &hadc, DistanceSensorArgs... distance_sensor_args)
    : _hadc(hadc) {
        std::tuple<DistanceSensorArgs...> tmp_args_tuple(distance_sensor_args...);
        // _arg_num = sizeof...(distance_sensor_args);  // 引数の数を得る
        saveDistanceSensors(tmp_args_tuple);
    }

    template<std::size_t index = 0, typename... Args>
    typename std::enable_if<index == sizeof...(Args), void>::type
    saveDistanceSensors(const std::tuple<Args...>&) {
        // インデックスがタプルのサイズと同じになったら何もしない
    }

    template<std::size_t index = 0, typename... Args>
    typename std::enable_if<index < sizeof...(Args), void>::type
    saveDistanceSensors(const std::tuple<Args...>& tuple) {
        _distance_sensor.push_back(std::get<index>(tuple));  // メンバ変数に格納する
        saveDistanceSensors<index + 1>(tuple);  // 再帰的に次の要素に移動
    }


    void update() {
        for (size_t i = 0; i < _distance_sensor.size(); i++) {
            HAL_ADC_Start(&_hadc);
            HAL_ADC_PollForConversion(&_hadc, 1);
            _distance_sensor[i].get()._adc_value = HAL_ADC_GetValue(&_hadc);
        }

    }

private:

    ADC_HandleTypeDef &_hadc;
    // std::vector<DistanceSensor> _distance_sensor;
    std::vector<std::reference_wrapper<DistanceSensor>> _distance_sensor;
    // size_t _arg_num;
};


}  // namespace mslh

#endif // MSLH_DISTANCE_SENSOR_STREAM_H
