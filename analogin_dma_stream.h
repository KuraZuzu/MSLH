/**
 * 2021/03/02
 * 改善したい点
 * これからインターフェース部分と裏でADC+DMAを回す切り分けを行う。
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H

#include "basis_analogin_dma_stream.h"

class AnalogInDMAStream {

    AnalogInDMAStream(ADC_HandleTypeDef& hadc, uint32_t rank, BasisAnalogInDMAStream& analog_in)
    : _hadc(hadc), _rank(rank), _shared_analog_in(analog_in)
    {}

    void init(){
        //ここで Basis のADC+DMAstartを開始する。
        _shared_analog_in.init(_hadc);
    }

    uint16_t read() {
        return _shared_analog_in.read(_hadc, _rank);
    }

private:
    ADC_HandleTypeDef& _hadc;
    uint32_t _rank;
//    BasisAnalogInDMAStream& _temp_init_analog_in;
    BasisAnalogInDMAStream& _shared_analog_in;
    //なんか参照型にできない.


};


#endif //ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
