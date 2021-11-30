/*!
 * @file gyro_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.07
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "arm_math.h"
#include "spi.h"
#include "dma.h"
#include "digital_out.h"
#include "stdio.h"

#ifndef MSLH_GYRO_SENSOR_H
#define MSLH_GYRO_SENSOR_H

namespace mslh {

class GyroSensor {

public:
    GyroSensor(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs_x, uint16_t cs_pin)
    : _hspi(hspi)
    , _cs_x(cs_x)
    , _cs_pin(cs_pin) {
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);
    }

    void start() {
    }

    void temp() {  //< 実験中
        MX_SPI3_Init();
        MX_DMA_Init();
        MX_GPIO_Init();
        HAL_SPI_Init(&hspi3);


        uint8_t rx_data[2] = {0x00, 0x00};  //< Reserved for receive data
        rx_data[0] = 0x00;
        rx_data[1] = 0x00;

        uint8_t reg = 0x75;   //< Input Hello::0x75 return(0x12)
        uint8_t data = 0x00;  //< No data for reading (Dummy data)
        uint8_t tx_data[2];
        tx_data[0] = 0x80 | reg;
        tx_data[1] = data;


        while(1) {
            HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_RESET);

            HAL_SPI_TransmitReceive_DMA(&hspi3, (uint8_t*)tx_data, (uint8_t*)rx_data, 2);
            while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY) { }
            HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);

            printf("0x%_x\r\n",rx_data);
            HAL_Delay(10);
        }
    }

private:
    SPI_HandleTypeDef &_hspi;
    GPIO_TypeDef *_cs_x;
    const uint16_t _cs_pin;
};

}  // namespace mslh

#endif //MSLH_GYRO_SENSOR_H