//
// Created by 倉澤　一詩 on 2021/03/02.
//

#ifndef ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H

#include "adc.h"


class AnalogInDMAStream {

public:
    AnalogInDMAStream(ADC_HandleTypeDef* hadc)
    :_hadc(hadc) ,_adc_amount(_hadc->Init.NbrOfConversion) {

        _value = new uint16_t[_adc_amount];
        for (uint32_t i = 0; i < _adc_amount; i++) _value[i] = 0;

        HAL_ADC_Start_DMA(_hadc, (uint32_t*)_value, _adc_amount);
    }

    /**
     * Note that the ranks defined in "adc.c" start from 1,
     * but in the arguments of this function, they start from 0.
     * This argument-rank is recommended to define it with enum.
     *
     * @argument Enter the rank (starting from 0) defined in "adc.c".
     *           It recommended to define it with enum.
     *
     * @param Returns the analog value with the resolution defined
     *        in "adc.c".(6~12[bit])
     */
    uint16_t read(uint16_t rank_starting_from_0) {
        return _value[rank_starting_from_0];
    }

private:
    ADC_HandleTypeDef* _hadc;
    uint32_t _adc_amount;
    uint16_t* _value;  // 6~12[bit]の範囲でデータが入力される．CubeMX(.ioc)のADCで設定．
};


#endif //ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
