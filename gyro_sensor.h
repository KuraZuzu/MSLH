/*!
 * @file gyro_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.07
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "spi.h"
#include "dma.h"
#include "digital_out.h"
#include "stdio.h"

#ifndef ZUZUHALFTPPMOD1_GYRO_SENSOR_H
#define ZUZUHALFTPPMOD1_GYRO_SENSOR_H

class GyroSensor {

public:
    GyroSensor(SPI_HandleTypeDef& hspi, DigitalOut cs_pin)
    : _hspi(hspi)
    , _cs_pin(cs_pin) {
        _cs_pin = 0;
    }

    void start() {
    }

    void temp() {  //< 実験中
        MX_SPI3_Init();
        MX_DMA_Init();
        MX_GPIO_Init();


        uint8_t rx_data[2] = {0x00, 0x00};  //< Reserved for receive data
        rx_data[0] = 0x00;
        rx_data[1] = 0x00;

        uint8_t reg = 0x75;   //< Input Hello::0x75 return(0x12)?
        uint8_t data = 0x00;  //< No data for reading
        uint8_t tx_data[2];
        tx_data[0] = reg | 0x80;
        tx_data[1] = data;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        HAL_SPI_Init(&hspi3);

        while(1) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

            HAL_SPI_TransmitReceive_DMA(&hspi3, (uint8_t*)tx_data, (uint8_t*)rx_data, 2);

            while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY) {
            }
            test_buzzer();

            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//        printf("%d\r\n",rx_data);
            printf("%04x\r\n",rx_data);
            HAL_Delay(10);
        }
    }

private:
    SPI_HandleTypeDef& _hspi;
    DigitalOut _cs_pin;
};


#endif //ZUZUHALFTPPMOD1_GYRO_SENSOR_H
