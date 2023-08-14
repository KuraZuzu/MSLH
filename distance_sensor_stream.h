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

/**
 * @warning
 *   STM32CubeMXでのADC設定必須項目
 *     Resolution                          : 12 bits (15 ADC Clock cycles)
 *     Data Alignment:                     : Right alignment
 *   　Scan Conversion Mode                : Enable
 *     Continuous Conversion Mode          : Disable
 *     Discontinuous Conversion Mode       : Enable
 *     Number Of Discontinuous Conversions : 1 (センサの数ではく"1"固定)
 *     DMA Continuous Requests             : Disable
 *     End Of Conversion Selection         : EOC flag at the end of single channel conversion
 *     Number Of Conversion                : 仕様する距離センサの数（"Rank"では変換するセンサの数だけ設定する）
 */


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
    friend class DistanceSensorStream;
};


class DistanceSensorStream {

public:

    template<typename... DistanceSensors>
    DistanceSensorStream(ADC_HandleTypeDef &hadc, DistanceSensors&... distance_sensors)
    : _hadc(hadc), _sensor_num(0) {
        (void)std::initializer_list<int>{(_distance_sensor.push_back(std::ref(distance_sensors)), 0)...};
        _sensor_num = _distance_sensor.size();  // get distance_sensor num
    }


    void update() {

        // LED OFF
        for(DistanceSensor &distance_sensor : _distance_sensor) distance_sensor._led.write(0);

        // sampling offset value
        uint16_t offset_values[_sensor_num];
        for(uint16_t &offset : offset_values) {
            HAL_ADC_Start(&_hadc);
            if(HAL_ADC_PollForConversion(&_hadc, 1) != HAL_OK) Error_Handler();
            offset = HAL_ADC_GetValue(&_hadc);
        }

        // LED ON
        for(DistanceSensor &distance_sensor : _distance_sensor) distance_sensor._led.write(1);
        HAL_Delay(1);  // 波形の立ち上がりを待つ("200[ns]"程度だが仮として"1[ms]"としている)

        // sampling peak value
        uint16_t peak_values[_sensor_num];
        for(uint16_t &peak : peak_values) {
            HAL_ADC_Start(&_hadc);
            if(HAL_ADC_PollForConversion(&_hadc, 1) != HAL_OK) Error_Handler();
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
