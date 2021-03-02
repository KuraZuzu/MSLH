/**
 * 2021/03/02
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H

#include "adc.h"

/**
 * Reads a analog value with an ADC using DMA.
 * The bit length specified by STM32CubeMX(.ico) for 3.3 volt peripherals.
 *
 * Example:
 * @code
 *
 * // It computes the battery voltage value when it is set
 * // at 12bit length and divided to 50% with 2×100[Ω] resistors by the electronic circuit.
 * // The rank of the pin for this battery check is set to "5",
 * // which corresponds to argument "4" [read(4)].
 *
 * #include "analogin_dma_stream"
 *
 * // define DMA-rank with enum.
 * enum Analog{
 *   LEFT_FRONT,
 *   LEFT_SIDE,
 *   RIGHT_SIDE,
 *   RIGHT_FRONT,
 *   BATTERY_VOLTAGE  // Target.
 * };
 *
 * AnalogInDMAStream analog(hadc1);
 *
 * int main() {
 *
 *     HAL_Init(); // Setup HAL.
 *     SystemClock_Config();  // Micro-controller startup functions
 *
 *     MX_ADC1_Init();  // Need setup ADC.
 *     MX_DMA_Init();   // Need setup DMA.
 *
 *
 *     analog.init ();  // It needs to be called after MX_ADC1_Init() and MX_DMA_Init().
 *
 *     uint16_t bat = analog.read(Analog::BATTERY_VOLTAGE);  // get_analog_value.
 *
 *     // The memory value is automatically updated by the ADC+DMA,
 *     // so the "bat" will always contain the latest value.
 *     // In addition, 0x0FFF is Max of 12bit.
 *     uint16_t voltage;
 *
 *     // [ setp 1 ]
 *     voltage = 3.3 * bat / 0x0FFF * (100 + 100)/100;
 *
 *     // [ step 2 ]
 *     voltage = 3.3 * bat / 0x0FFF * (100 + 100)/100;  // "bat" and "voltage" has different value at [ step 1 ].
 *
 * }
 * @endcode
 */
class AnalogInDMAStream {

public:
    AnalogInDMAStream(ADC_HandleTypeDef& hadc);


    /**
     * 本当はコンストラクタ内で ADC を開始したい。
     * ペリフェラルの初期化を先に行うことが必須となってしまうために
     * ADC の開始を init() に切り分けた。
     */
    void init();


    /**
     * Note that the ranks defined in "adc.c" start from 1,
     * but in the arguments of this function, they start from 0.
     * This argument-rank is recommended to define it with enum.
     *
     * In the sample code, the rank in adc.c is 5,
     * but in enum it is 4
     * (because rank starts from 1, but enum starts from 0).
     *
     *
     * @argument Enter the rank (starting from 0) defined in "adc.c".
     *           It recommended to define it with enum.
     *
     * @param Returns the analog value with the resolution defined
     *        in "adc.c".(6~12[bit])
     */
    uint16_t read(uint16_t rank_starting_from_0);

private:
    ADC_HandleTypeDef& _hadc;
    uint32_t* _adc_amount;
    uint16_t* _value;  // 6~12[bit]の範囲でデータが入力される．CubeMX(.ioc)のADCで設定．
    bool _init_flag;
};


#endif //ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
