/*!
 * @file adc1_dma_stream.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.11
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "adc1_dma_stream.h"

namespace adc1_dma_stream {
    constexpr uint32_t _adc1_buffer = 5;  //< hadc1.Init.NbrOfConversion;
    uint16_t _adc1_dma_value[_adc1_buffer] = {0, 0, 0, 0, 0};
    uint16_t _offset_adc1_dma_value = 0;
    static bool _adc1_active_flag = false;
}


void adc1_dma_stream::init_adc1_dma_stream() {

    if(!_adc1_active_flag) {
        MX_ADC1_Init();
        MX_DMA_Init();
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *) _adc1_dma_value, _adc1_buffer);
        _adc1_active_flag = true;
    }
}

uint16_t adc1_dma_stream::adc_read(uint16_t adc1_rank) {
    return _adc1_dma_value[adc1_rank - 1];
}
