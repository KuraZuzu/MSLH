/**
 * 2021/03/02
 * @author KuraZuzu
 */

#include "analogin_dma_stream.h"

AnalogInDMAStream::AnalogInDMAStream(ADC_HandleTypeDef& hadc)
: _hadc(hadc)
, _executing_flag(false)
, _adc_amount(_hadc.Init.NbrOfConversion)
{
}

void AnalogInDMAStream::start() {

    if(!_executing_flag) {

        _value = new uint16_t[_adc_amount];
        for (uint32_t i = 0; i < _adc_amount; i++) {
            _value[i] = 0;
        }

        HAL_ADC_Start_DMA(&_hadc, (uint32_t *) _value, _adc_amount);
        _executing_flag = true;
    }
}

uint16_t AnalogInDMAStream::read(uint16_t rank_starting_from_0) {
    return _value[rank_starting_from_0];
}
