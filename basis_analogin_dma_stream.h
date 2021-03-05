//
// Created by 倉澤　一詩 on 2021/03/05.
//

#ifndef ZUZUHALFTPPMOD1_BASIS_ANALOGIN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_BASIS_ANALOGIN_DMA_STREAM_H


#include "adc.h"

class BasisAnalogInDMAStream {

public:

    BasisAnalogInDMAStream(){}

    void init(ADC_HandleTypeDef& hadc) {

        if(hadc.Instance == ADC1 && !_active_ADC1_flag) {
            _adc1_value = new uint16_t[hadc.Init.NbrOfConversion];
            for (uint32_t i = 0; i < hadc.Init.NbrOfConversion; ++i) _adc1_value[i] = 0;
            HAL_ADC_Start_DMA(&hadc, (uint32_t*)_adc1_value, hadc.Init.NbrOfConversion);
            _active_ADC1_flag = true;

        }else if (hadc.Instance == ADC2 && !_active_ADC2_flag) {
            _adc2_value = new uint16_t[hadc.Init.NbrOfConversion];
            for (uint32_t i = 0; i < hadc.Init.NbrOfConversion; ++i) _adc2_value[i] = 0;
            HAL_ADC_Start_DMA(&hadc, (uint32_t*)_adc2_value, hadc.Init.NbrOfConversion);
            _active_ADC2_flag = true;

        }else if (hadc.Instance == ADC3 && !_active_ADC3_flag) {
            _adc3_value = new uint16_t[hadc.Init.NbrOfConversion];
            for (uint32_t i = 0; i < hadc.Init.NbrOfConversion; ++i) _adc3_value[i] = 0;
            HAL_ADC_Start_DMA(&hadc, (uint32_t*)_adc3_value, hadc.Init.NbrOfConversion);
            _active_ADC3_flag = true;

        }

    }

    uint16_t read(ADC_HandleTypeDef& hadc, uint32_t rank) {
        if(hadc.Instance == ADC1) return _adc1_value[rank];
        else if(hadc.Instance == ADC1) return _adc2_value[rank];
        else if(hadc.Instance == ADC1) return _adc3_value[rank];
        else return 0;
    }

private:
    // 全部static?
    static uint16_t* _adc1_value;
    static uint16_t* _adc2_value;
    static uint16_t* _adc3_value;
    static bool _active_ADC1_flag;
    static bool _active_ADC2_flag;
    static bool _active_ADC3_flag;

};



#endif //ZUZUHALFTPPMOD1_BASIS_ANALOGIN_DMA_STREAM_H
