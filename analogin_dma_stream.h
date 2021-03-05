/**
 * 2021/03/02
 * 改善したい点
 * これからインターフェース部分と裏でADC+DMAを回す切り分けを行う。
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H

#include "adc.h"

class AnalogInDMAStream {

public:
    AnalogInDMAStream(ADC_HandleTypeDef& hadc, uint32_t rank)
    : _hadc(hadc), _rank(rank)
    {}

    void start(){
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

    uint16_t read() {
        if(_hadc.Instance == ADC1) return _adc1_value[_rank - 1];
        else if(_hadc.Instance == ADC2) return _adc2_value[_rank - 1];
        else if(_hadc.Instance == ADC3) return _adc3_value[_rank - 1];
        else return 0;
    }

private:
    ADC_HandleTypeDef& _hadc;
    uint32_t _rank;
    static uint16_t* _adc1_value;
    static uint16_t* _adc2_value;
    static uint16_t* _adc3_value;
    static bool _active_ADC1_flag;
    static bool _active_ADC2_flag;
    static bool _active_ADC3_flag;

};


#endif //ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
