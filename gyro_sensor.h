/*!
 * @file gyro_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.07
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_GYRO_SENSOR_H
#define MSLH_GYRO_SENSOR_H

#include "arm_math.h"
#include "spi.h"
#include "dma.h"
#include "digital_out.h"
#include "stdio.h"


namespace mslh {

class GyroSensorICM42688P {

public:
    GyroSensorICM42688P(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs_x, uint16_t cs_pin)
    : _hspi(hspi)
    , _cs_x(cs_x)
    , _cs_pin(cs_pin) {
        // HAL_SPI_Init(&hspi);
        // HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);
    }

    void init() {
        HAL_SPI_Init(&_hspi);
    }



    uint8_t read(void) {

        const uint8_t mode = 0b10000000;  //< read:1, write:0
        const uint8_t reg = 117;   //< Input Hello::0x75 return(0x12)
        const uint8_t data = 0x00;  //< No data for reading (Dummy data)
        uint8_t tx_data[2] = {(mode|reg), data};
        uint8_t rx_data[2] = {0x00, 0x00};  //< Reserved for receive data

        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive_DMA(&_hspi, (uint8_t*)tx_data, (uint8_t*)rx_data, sizeof(tx_data));
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);
        const uint16_t return_rx_data = rx_data[0];
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // SS/CS pin is made HIGH

        return rx_data[1];  // 受け取るのは2バイト目のデータだけ
    }


private:
    SPI_HandleTypeDef &_hspi;
    GPIO_TypeDef *_cs_x;
    const uint16_t _cs_pin;
};

}  // namespace mslh

#endif //MSLH_GYRO_SENSOR_H
