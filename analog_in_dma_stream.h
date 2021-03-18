/*!
 * @file analogin_dma_stream.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.02
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */


/**
 *
 * This class deals with ADCs using DMA.
 *
 * 問題点
 * ・ラッパ関数内でインスタンス生成しないとコンストラクタが呼ばれない．
 *  他のラッパ関数内でMachineのインスタンスを生成するとAnalogInが読めない．
 *  AnalogInについて，同じラッパ関数内で何回もインスタンスを生成しても読めるが，
 *  他の関数でインスタンス生成していると読めない．バグ．
 *
 * ・ADC+DMA でアナログ値が入力されるメモリの配列は複数確保できないため，共有される必要がある．
 * 　start() を呼び出した際に，同じハンドラのADCも自動的に開始されてしまう．
 * 　つまり，start() を呼んでいないインスタンスでも計測が開始されてしまう．(実行上での不具合は無い)
 * 　オブジェクト指向の基本である内部を秘匿とする行為に置いて，これは完全なバグと同様である．．
 * 　しかし，そういう機能である以上仕方ないとも言える．
 *
 * ・start() があるのに stop() が無いのは直感的な分かり易さに反する。
 * 　しかし，init() にすると ADCとDMA のInit() よりも早く宣言しても違和感が無くなってしまう．
 * 　こちらも直感的な分かり易さに反する．
 *
 * ・以前に同様のADCハンドラから start() が呼び出された場合，既にADC+DMAの転送先である配列確保
 * 　と計測開始の関数をスキップするようにできている．
 * 　これは，入力されたハンドラが過去に呼び出されたものと同様かチェックして，既に起動しているかフラグで管理している．
 * 　ただし，ADC1~ADC3 までしか対応しておらず，これ以上の範囲が追加された際に手動での追加が必要となる．
 * 　これは，明確な欠点であり美しくない．
 *
 * @date 2021.03.02
 * @author Kazushi Kurasawa
 */

#ifndef ZUZUHALFTPPMOD1_ANALOG_IN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ANALOG_IN_DMA_STREAM_H

#include "adc.h"
#include "dma.h"

/**
 * @brief
 *   Reads a analog value with an ADC using DMA.
 *
 * @note
 *   The bit length specified by STM32CubeMX(.ico) for 3.3 volt peripherals.
 *
 * Example:
 * @code
 *   // It computes the battery voltage value when it is set at 12bit length and divided
 *   // to 50% with 2×100[Ω] resistors by the electronic circuit.
 *   // in this case, the rank of the pin for this battery check is set to "5",
 *
 *   #include "analogin_dma_stream"
 *
 *   AnalogInDMAStream analog(hadc1, 5);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 *
 *       MX_ADC1_Init();  //< Need setup ADC.
 *       MX_DMA_Init();   //< Need setup DMA.
 *
 *       analog.start();  //< It needs to be called after MX_ADC1_Init() and MX_DMA_Init().
 *
 *       uint16_t bat = analog.read();  //< get_analog_value.
 *
 *       // The memory value is automatically updated by the ADC+DMA,
 *       // so the "bat" will always contain the latest value.
 *       // In addition, 0x0FFF is Max of 12bit.
 *       uint16_t voltage;
 *
 *       // [ setp 1 ]
 *       voltage = 3.3 * bat / 0x0FFF * (100 + 100)/100;
 *
 *       // [ step 2 ]
 *       voltage = 3.3 * bat / 0x0FFF * (100 + 100)/100;  //< "bat" and "voltage" has different value at [ step 1 ].
 *   }
 * @endcode
 */
class AnalogInDMAStream {

public:
    /**
     * @param (uint32_t rank) is the ACD priority, it is set by STM32CubeMX(.ioc)
     */
    AnalogInDMAStream(ADC_HandleTypeDef &hadc, uint32_t rank);

    /**
     * @fn
     *   Start ADC measurement.  <br>
     *
     * @details
     *   This function needs to be called after ADC and DMA init_functions are called.  <br>
     *   Example: MX_DMA_Init(), MX_ADC1_Init().  <br>
     *     <br>
     *   ADC と DMA の Init関数 を呼び出した後に実行してください。
     */
    void start();

    /**
     * @fn Get analog value.
     * @return value size is unsigned_int 0~12[bit] (0x0FFF).
     */
    inline uint16_t read() const {
        return _adc_value[_adc_x][_rank];
    }

private:
    uint16_t _adc_x;
    ADC_HandleTypeDef& _hadc;
    const uint32_t _rank;  //< 本来はランクは1からはじまるが，配列の最初の0からにオフセット．
    static bool _active_flag[3];  //< ADC1, ADC2, ADC3 の３つ分
    static uint16_t *_adc_value[3];  //< ADC1, ADC2, ADC3 の３つ分
};

#endif //ZUZUHALFTPPMOD1_ANALOG_IN_DMA_STREAM_H
