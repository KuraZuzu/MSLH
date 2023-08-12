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

private:
    DigitalOut _led;
    uint16_t _adc_value;
    template<typename... DistanceSensors> friend class DistanceSensorStream;
};

/**
* @warning
*   DistanceSensorのrankは以下のように設定してください
*   ・１つのチャネルのセンサのみを引数に与えてください
*   ・１つのセンサにつきrank2つぶんを連続で設定してください
*   ・センサ１、センサ２、センサ３を設定する場合
*/
template<typename... DistanceSensors>
class DistanceSensorStream {

public:

    DistanceSensorStream(ADC_HandleTypeDef &hadc, DistanceSensors&... distance_sensors)
    : _hadc(hadc), _sensor_num(0) {
        (void)std::initializer_list<int>{(_distance_sensor.push_back(std::ref(distance_sensors)), 0)...};
        _sensor_num = _distance_sensor.size();  // 距離センサのサイズを取得
    }


    void update() {

        // LED消灯
        for(DistanceSensor &distance_sensor : _distance_sensor) distance_sensor._led.write(0);

        // オフセットの値を取得
        uint16_t offset_values[_sensor_num];
        for(uint16_t &offset : offset_values) {
            HAL_ADC_Start(&_hadc);
            HAL_ADC_PollForConversion(&_hadc, 1);
            offset = HAL_ADC_GetValue(&_hadc);
        }

        // LED点灯
        for(DistanceSensor &distance_sensor : _distance_sensor) distance_sensor._led.write(1);

        // ピークの値を取得
        uint16_t peak_values[_sensor_num];
        for(uint16_t &peak : peak_values) {
            HAL_ADC_Start(&_hadc);
            HAL_ADC_PollForConversion(&_hadc, 1);
            peak = HAL_ADC_GetValue(&_hadc);
        }

        for (size_t i = 0; i < _sensor_num; i++) {
            _distance_sensor[i].get()._adc_value = peak_values[i] - offset_values[i];
        }
    }

private:

    ADC_HandleTypeDef &_hadc;
    std::vector<std::reference_wrapper<DistanceSensor>> _distance_sensor;
    size_t _sensor_num;
};


}  // namespace mslh

#endif // MSLH_DISTANCE_SENSOR_STREAM_H
