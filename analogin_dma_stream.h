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
    AnalogInDMAStream(ADC_HandleTypeDef& hadc, uint32_t rank):_rank(rank)
    {
        if( (hadc.Instance==ADC1 && !_active_ADC1_flag)
            || (hadc.Instance==ADC2 && !_active_ADC2_flag)
            || (hadc.Instance==ADC3 && !_active_ADC3_flag) ) {

            _value = new uint16_t[hadc.Init.NbrOfConversion];
            for (uint32_t i = 0; i < hadc.Init.NbrOfConversion; ++i) _value[i] = 0;

            HAL_ADC_Start_DMA(&hadc, (uint32_t *) _value, hadc.Init.NbrOfConversion);
        }

    }

    void start() {}

    uint16_t read() {
        return _value[_rank - 1];
    }

private:
    static bool _active_ADC1_flag;
    static bool _active_ADC2_flag;
    static bool _active_ADC3_flag;
    uint32_t _rank;
    uint16_t *_value;

};


#endif //ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
