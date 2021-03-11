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

enum ADC1_Rank{
    LEFT_FRONT = 1,
    LEFT_SIDE = 2,
    RIGHT_SIDE = 3,
    RIGHT_FRONT = 4,
    BATTERY = 5
};


namespace adc1_dma_stream {

    void init_adc1_dma_stream();

    uint16_t adc_read(uint16_t adc1_rank);
}

#endif //ZUZUHALFTPPMOD1_ADC1_DMA_STREAM_H
