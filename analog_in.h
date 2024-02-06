/*!
 * @file analogin_dma_stream.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2023.08.10
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_ANALOG_IN_H
#define MSLH_ANALOG_IN_H

/**
 * @warning
 *   STM32CubeMXでのADC設定必須項目
 *     Resolution                          : 12 bits (15 ADC Clock cycles)
 *     Data Alignment:                     : Right alignment
 *   　Scan Conversion Mode                : Enable
 *     Continuous Conversion Mode          : Disable
 *     Discontinuous Conversion Mode       : Enable
 *     Number Of Discontinuous Conversions : 1 (ADCの数ではく"1"固定)
 *     DMA Continuous Requests             : Disable
 *     End Of Conversion Selection         : EOC flag at the end of single channel conversion
 *     Number Of Conversion                : 仕様する距離ADCの数（"Rank"では変換するセンサの数だけ設定する）
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include "arm_math.h"
#include "adc.h"

namespace mslh {


class AnalogIn {

public:
    AnalogIn(const uint32_t adc_rank): _adc_rank(adc_rank), _adc_value(0) {}

    inline uint16_t read() { return _adc_value; }

private:
    const uint32_t _adc_rank;
    uint16_t _adc_value;
    friend class AnalogInUpdater;
};


class AnalogInUpdater
{

public:
    template <typename... AnalogIns>
    AnalogInUpdater(ADC_HandleTypeDef &hadc, AnalogIns &...analog_ins): _hadc(hadc) {

        std::vector<std::reference_wrapper<AnalogIn>> temp_analog_ins{analog_ins...};
        sortAndVerify(temp_analog_ins);
        _analog_in = temp_analog_ins;  // rank順にソートした配列
    }

    void update() {
        for(AnalogIn &value : _analog_in) {
            HAL_ADC_Start(&_hadc);
            if(HAL_ADC_PollForConversion(&_hadc, 1) != HAL_OK) Error_Handler();
            value._adc_value = HAL_ADC_GetValue(&_hadc);
        }
    }

private:

    void sortAndVerify(std::vector<std::reference_wrapper<AnalogIn>> &analog_ins)
    {
        // _adc_rank の順番に従ってソート
        std::sort( analog_ins.begin(), analog_ins.end(), [](const AnalogIn &a, const AnalogIn &b) {
            return a._adc_rank < b._adc_rank; } );

        // Rankが1から始まり連続した整数値でないときにエラーとなる
        for (size_t i = 0; i < analog_ins.size(); ++i) {
            if (analog_ins[i].get()._adc_rank != i + 1) {
                // ここでエラー処理
                Error_Handler();
            }
        }
    }


    ADC_HandleTypeDef &_hadc;
    std::vector<std::reference_wrapper<AnalogIn>> _analog_in;
};


} // namespace mslh

#endif // MSLH_ANALOG_IN_H
