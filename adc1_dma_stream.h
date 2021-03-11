/*!
 * @file adc1_dma_stream.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.11
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_ADC1_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ADC1_DMA_STREAM_H

#include "adc.h"
#include "dma.h"

namespace adc1_dma_stream {

    enum ADC1_Rank{
        LEFT_FRONT,  // 0
        LEFT_SIDE,
        RIGHT_SIDE,
        RIGHT_FRONT,
        BATTERY_VOLTAGE
    };

    constexpr uint32_t _adc1_buffer = 5;  //< hadc1.Init.NbrOfConversion
    uint16_t _adc1_dma_value[_adc1_buffer] = {0, 0, 0, 0, 0};
    uint16_t _offset_adc1_dma_value;

    void initAdc1DmaStream();

    uint16_t read(uint16_t adc1_rank_starting_from_0);
}

#endif //ZUZUHALFTPPMOD1_ADC1_DMA_STREAM_H
