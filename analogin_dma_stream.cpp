/**
 * This class deals with ADCs using DMA.
 *
 * @date 2021.03.02
 * @author Kazushi Kurasawa
 */

#include "analogin_dma_stream.h"

AnalogInDMAStream::AnalogInDMAStream(ADC_HandleTypeDef &hadc, uint32_t rank)
        : _hadc(hadc), _rank(rank) {
}

void AnalogInDMAStream::start() {
    if(_hadc.Instance == ADC1 && !_active_ADC1_flag) {
        _adc1_value = new uint16_t[_hadc.Init.NbrOfConversion];
        for (uint32_t i = 0; i < _hadc.Init.NbrOfConversion; ++i) _adc1_value[i] = 0;
        HAL_ADC_Start_DMA(&_hadc, (uint32_t*)_adc1_value, _hadc.Init.NbrOfConversion);
        _active_ADC1_flag = true;

    }else if (_hadc.Instance == ADC2 && !_active_ADC2_flag) {
        _adc2_value = new uint16_t[_hadc.Init.NbrOfConversion];
        for (uint32_t i = 0; i < _hadc.Init.NbrOfConversion; ++i) _adc2_value[i] = 0;
        HAL_ADC_Start_DMA(&_hadc, (uint32_t*)_adc2_value, _hadc.Init.NbrOfConversion);
        _active_ADC2_flag = true;

    }else if (_hadc.Instance == ADC3 && !_active_ADC3_flag) {
        _adc3_value = new uint16_t[_hadc.Init.NbrOfConversion];
        for (uint32_t i = 0; i < _hadc.Init.NbrOfConversion; ++i) _adc3_value[i] = 0;
        HAL_ADC_Start_DMA(&_hadc, (uint32_t*)_adc3_value, _hadc.Init.NbrOfConversion);
        _active_ADC3_flag = true;
    }
}

uint16_t AnalogInDMAStream::read() const {
    if(_hadc.Instance == ADC1) return _adc1_value[_rank - 1];
    else if(_hadc.Instance == ADC2) return _adc2_value[_rank - 1];
    else if(_hadc.Instance == ADC3) return _adc3_value[_rank - 1];
    else return 0;
}


uint16_t* AnalogInDMAStream::_adc1_value;
uint16_t* AnalogInDMAStream::_adc2_value;
uint16_t* AnalogInDMAStream::_adc3_value;
bool AnalogInDMAStream::_active_ADC1_flag = false;
bool AnalogInDMAStream::_active_ADC2_flag = false;
bool AnalogInDMAStream::_active_ADC3_flag = false;