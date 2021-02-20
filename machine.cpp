/**
 * 2021/02/13
 * @author KuraZuzu
 * */

#include "machine.h"


Machine::Machine()
:_led1(GPIOC, GPIO_PIN_3), _led2(GPIOC, GPIO_PIN_4), _led3(GPIOC, GPIO_PIN_5){

    /** Init functions are needed at first of implement of machine front-end constructor
     * STM32CubeMX によって生成された Init関数 は，ライブラリから持ってきた最初の段階で(ライブラリの実態が生成される前)
     * に呼び出されている必要があります．
     * (ライブラリとしての汎用性や意図しない動作を引き起こさないためにも改良すべき)
     *
     * @author KuraZuzu */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM8_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_USART2_UART_Init();
    MX_SPI3_Init();
    MX_TIM6_Init();
    MX_TIM7_Init();
    MX_TIM5_Init();

    _l_motor = new Motor(GPIOA, GPIO_PIN_6, &htim1, TIM_CHANNEL_1, static_cast<GPIO_PinState >(true));
    _r_motor = new Motor(GPIOA, GPIO_PIN_7, &htim1, TIM_CHANNEL_2, static_cast<GPIO_PinState >(false));
//    _l_motor = new Motor(GPIOA, GPIO_PIN_6, &htim1, TIM_CHANNEL_1, GPIO_PIN_SET);
//    _r_motor = new Motor(GPIOA, GPIO_PIN_7, &htim1, TIM_CHANNEL_2, GPIO_PIN_RESET);
    _l_encoder = new Encoder(&htim4, true);
    _r_encoder = new Encoder(&htim3, false);
    _buzzer = new Buzzer(&htim8, TIM_CHANNEL_1);

    stop();
}

void Machine::move_debug() {
    _l_motor->update(0.5);
    _r_motor->update(0.5);
}

void Machine::stop() {
    _l_motor->update(0);
    _r_motor->update(0);
}

void Machine::buzzer_debug() {
    _buzzer->pi();
    HAL_Delay(1000);
    _buzzer->pipi();
    HAL_Delay(1000);
    _buzzer->pipipi();
}

void Machine::led_debug() {
    while (1) {
        _led1 = !_led1;
        _led2 = !_led2;
        _led3 = !_led3;
        HAL_Delay(200);
    }
}
