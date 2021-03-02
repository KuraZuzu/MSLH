//
// Created by 倉澤　一詩 on 2021/03/02.
//

#ifndef ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H
#define ZUZUHALFTPPMOD1_ANALOGIN_DMA_STREAM_H

#include "adc.h"

/**
 * Turns the motor at the specified PWM rate.
 *
 * Example:
 * @code
 *
 * // The motor is rotated forward and backward at a PWM value of 0.5 (50% output) for
 * // 3 seconds each, and then stopped.
 *
 * #include "motor.h"
 *
 * Motor motor(GPIOA, GPIO_PIN_6, htim1, TIM_CHANNEL_1, true);
 *
 * int main() {
 *
 *     MX_TIM1_Init();  // Need setup HAL encoder timer parameters.
 *     MX_GPIO_Init();  // Need setup HAL_GPIO.
 *
 *     motor.update(0.5);  // Roted forward with PWM of 50% output.
 *     HAL_Delay(3000);
 *
 *     motor.update(-0.5); // Roted backward with PWM of 50% output.
 *     HAL_Delay(3000);
 *
 *     motor.update(0.0);  // Stop motor.
 * }
 * @endcode
 */
class AnalogInDMAStream {

public:
    AnalogInDMAStream(ADC_HandleTypeDef& hadc);


    /**
     * 本当はコンストラクタ内で ADC を開始したい。
     * ペリフェラルの初期化を先に行うことが必須となってしまうために
     * ADC の開始を init() に切り分ける。s
     */
    void init();


    /**
     * Note that the ranks defined in "adc.c" start from 1,
     * but in the arguments of this function, they start from 0.
     * This argument-rank is recommended to define it with enum.
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
