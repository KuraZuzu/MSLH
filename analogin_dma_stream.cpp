//
// Created by 倉澤　一詩 on 2021/03/04.
//

#include "analogin_dma_stream.h"

uint16_t* AnalogInDMAStream::_adc1_value;
uint16_t* AnalogInDMAStream::_adc2_value;
uint16_t* AnalogInDMAStream::_adc3_value;
bool AnalogInDMAStream::_active_ADC1_flag = false;
bool AnalogInDMAStream::_active_ADC2_flag = false;
bool AnalogInDMAStream::_active_ADC3_flag = false;