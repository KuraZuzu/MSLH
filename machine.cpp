/**
 * 2021/02/13
 * @author KuraZuzu
 * */

#include "machine.h"


Machine::Machine()
:_led1(GPIOC, GPIO_PIN_3), _led2(GPIOC, GPIO_PIN_4), _led3(GPIOC, GPIO_PIN_5)
{

    /** Init functions. */
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
    _l_encoder = new Encoder(&htim4, 500*4, false);
    _r_encoder = new Encoder(&htim3, 500*4, true);
    _buzzer = new Buzzer(&htim8, TIM_CHANNEL_1);
    _serial = new SerialUtility(&huart2);
    stop();
}

void Machine::stop() {
    _l_motor->update(0);
    _r_motor->update(0);
}

void Machine::move_debug() {
    _l_motor->update(0.05);
    _r_motor->update(0.05);
    HAL_Delay(1000);
    _l_motor->update(-0.05);
    _r_motor->update(0.05);
    HAL_Delay(1000);
    _l_motor->update(0.05);
    _r_motor->update(-0.05);
    HAL_Delay(1000);
    stop();
}

void Machine::buzzer_debug() {
    _buzzer->pipi();
}

void Machine::led_debug() {
    _led1 = 1;
    _led2 = 0;
    _led3 = 0;
    HAL_Delay(50);
    _led1 = 0;
    _led2 = 1;
    _led3 = 0;
    HAL_Delay(50);
    _led1 = 0;
    _led2 = 0;
    _led3 = 1;
    HAL_Delay(50);
    _led1 = 0;
    _led2 = 0;
    _led3 = 0;
}

void Machine::serial_debug() {
    int i = 0;
    while(1) {
        printf("%d", i);
        printf(": sucess\r\n");
        i++;
    }
}

void Machine::encoder_debug() {
    _l_encoder->start();
    _r_encoder->start();
    int i = 0;
    while (1) {
        _l_encoder->update();
        _r_encoder->update();
        printf("LP:%d, LC:%d,  RP:%d, RC:%d\r\n", static_cast<int>(_l_encoder->get_surplus_pulse()),
               static_cast<int>(_l_encoder->get_rotation_count()), static_cast<int>(_r_encoder->get_surplus_pulse()),
               static_cast<int>(_r_encoder->get_rotation_count()));
        HAL_Delay(10);
        i++;
    }

}



/**
*int main() {
*
*    // *** Need setup HAL encoder timer parameters. ***
*    MX_TIM3_Init();
 *
*    // [ STEP 0 ]
*    int64_t delta_pulse;
*    int64_t rotation_count;
*    encoder.start(); // Init encoder setup.*
*
*
*    // [ STEP 1 ]
*    encoder.update();
*    delta_pulse = encoder.get_delta_pulse();       // delta_pulse between step 0 and 1
*    rotation_count = encoder.get_rotation_count(); // rotation count when step 1*
*
*    // *** Need encoder.update(); , if you get current data. ***
*    delta_pulse = encoder.get_delta_pulse();       // delta_pulse between step 0 and 1
*    rotation_count = encoder.get_rotation_count(); // rotation count when step 1
*
*
*    // [ STEP 2 ]
*    encoder.update();
*    delta_pulse = encoder.get_delta_pulse();       // delta_pulse between step 1 and 2
*    rotation_count = encoder.get_rotation_count(); // rotation count when step 2
*}
*/