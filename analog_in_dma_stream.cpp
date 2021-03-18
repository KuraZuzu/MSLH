/*!
 * @file analogin_dma_stream.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.02
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */


/**
 * This class deals with ADCs using DMA.
 */
#include "analog_in_dma_stream.h"

AnalogInDMAStream::AnalogInDMAStream(ADC_HandleTypeDef &hadc, uint32_t rank)
        : _hadc(hadc), _rank(rank-1), _adc_x(0) {
}

void AnalogInDMAStream::start() {

    if(_hadc.Instance == ADC1) _adc_x = 0;
    else if(_hadc.Instance == ADC2) _adc_x = 1;
    else if(_hadc.Instance == ADC3) _adc_x = 2;
    // このどれにも当てはまらない場合はエラーを投げたい．

    if (!_active_flag[_adc_x]) {
        _adc_x = 0;  //< ADC1だが，0からのインデックスなので (1-1 = 0)．
        _adc_value[_adc_x] = new uint16_t[_hadc.Init.NbrOfConversion];
        for (uint32_t i = 0; i < _hadc.Init.NbrOfConversion; ++i) _adc_value[_adc_x][i] = 0;
        HAL_ADC_Start_DMA(&_hadc, (uint32_t *) _adc_value[_adc_x], _hadc.Init.NbrOfConversion);

        _active_flag[_adc_x] = true;
    }
}

bool AnalogInDMAStream::_active_flag[3] = {false, false, false};  //<  ADC1, ADC2, ADC3 の３つ分
uint16_t *AnalogInDMAStream::_adc_value[3];  //< ADC1, ADC2, ADC3 の３つ分