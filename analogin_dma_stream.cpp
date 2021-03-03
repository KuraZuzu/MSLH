/**
 * 2021/03/02
 * @author KuraZuzu
 */

#include "analogin_dma_stream.h"

AnalogInDMAStream::AnalogInDMAStream(ADC_HandleTypeDef& hadc, uint16_t adc_buffer)
: _hadc(hadc)
, _executing_flag(false)
, _adc_buffer(adc_buffer)
{
    _value = new uint16_t[_adc_buffer];
}


void AnalogInDMAStream::start() {

    if(!_executing_flag) {
        HAL_ADC_Start_DMA(&_hadc, (uint32_t *) _value, _adc_buffer);
        _executing_flag = true;
    }
}

void AnalogInDMAStream::stop() {
    if(_executing_flag) {
        HAL_ADC_Stop_DMA(&_hadc);
        _executing_flag = false;
    }
}

uint16_t AnalogInDMAStream::read(uint16_t rank_starting_from_0) {
    return _value[rank_starting_from_0];
}
