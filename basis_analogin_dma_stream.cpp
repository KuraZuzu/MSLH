//
// Created by 倉澤　一詩 on 2021/03/05.
//

#include "basis_analogin_dma_stream.h"

uint16_t*  BasisAnalogInDMAStream::_adc1_value;
uint16_t*  BasisAnalogInDMAStream::_adc2_value;
uint16_t*  BasisAnalogInDMAStream::_adc3_value;
bool BasisAnalogInDMAStream::_active_ADC1_flag = false;
bool BasisAnalogInDMAStream::_active_ADC2_flag = false;
bool BasisAnalogInDMAStream::_active_ADC3_flag = false;