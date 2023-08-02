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
    }

    void init() {
        HAL_SPI_Init(&_hspi);
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);
    }

    uint8_t getWhoAmI() {

        const uint16_t data_size = 2;

        const uint8_t mode_rw = 0b10000000;  //< read:1, write:0
        const uint8_t reg = 0x75;   //< Input who_am_i:    0x return: 0x47

        uint8_t tx_data[data_size];
        tx_data[0] = mode_rw | reg;
        tx_data[1] = 0x00;  //< Dummy data
        uint8_t rx_data[data_size] = {0x00, 0x00};

        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_RESET);  // SS/CS pin : LOW
        HAL_SPI_TransmitReceive(&_hspi, (uint8_t*)tx_data, (uint8_t*)rx_data, data_size, 100);
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);

        return rx_data[1];  // 受け取るのは2バイト目のデータだけ
    }


private:
    SPI_HandleTypeDef &_hspi;
    GPIO_TypeDef *_cs_x;
    const uint16_t _cs_pin;
};

}  // namespace mslh

#endif //MSLH_GYRO_SENSOR_H
