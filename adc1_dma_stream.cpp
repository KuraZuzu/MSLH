/*!
 * @file adc1_dma_stream.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.11
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "adc1_dma_stream.h"

void adc1_dma_stream::initAdc1DmaStream() {
    MX_ADC1_Init();
    MX_DMA_Init();
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)_adc1_dma_value, _adc1_buffer);
}

uint16_t adc1_dma_stream::read(uint16_t adc1_rank_starting_from_0) {
    return _adc1_dma_value[adc1_rank_starting_from_0];
}
