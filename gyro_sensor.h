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
        setConfigs();
    }

    uint8_t getWhoAmI() {
        const uint8_t address = 0x75;   //< Input who_am_i: 0x75, return: 0x47
        return read(address);  // 受け取るのは2バイト目のデータだけ
    }

    float32_t getAccelZ() {
        // return (2000.0f * 9.8f * static_cast<float32_t>(getRawAccelZ() / UINT16_MAX));
        return static_cast<float32_t>(getRawAccelZ());
    }

    uint16_t getRawAccelZ() {
        // 0x23: upper Byte,  0x24: lower Byte
        // return read2B(0x23, 0x24, 3);
        return (read(0x23) << 8) + read(0x24);
    }

private:
    uint8_t read(const uint8_t address) {

        constexpr uint16_t size = 2;
        const uint8_t read_mode = 0b10000000;  //< (read:1, write:0)
        const uint8_t tx_data[size] = {static_cast<uint8_t>(read_mode | address), 0x00};
        uint8_t rx_data[size] = {0x00, 0x00};

        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_RESET);  // CS pin: LOW
        HAL_SPI_TransmitReceive(&_hspi, (uint8_t*)tx_data, (uint8_t*)rx_data, size, 100);
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);

        return rx_data[1];  //< データは2バイト目に格納される
    }

    // バースト読み取りテスト
    // uint16_t read2B(const uint8_t addr_0, const uint8_t addr_1, const uint16_t buff) {

    //     const uint16_t size = buff;
    //     const uint8_t read_mode = 0b10000000;  //< (read:1, write:0)
    //     const uint8_t tx_data[size] = {static_cast<uint8_t>(read_mode | addr_0), static_cast<uint8_t>(read_mode | addr_1), 0x00};
    //     uint8_t rx_data[size];

    //     HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_RESET);  // CS pin: LOW
    //     HAL_SPI_TransmitReceive(&_hspi, (uint8_t*)tx_data, (uint8_t*)rx_data, size, 100);
    //     HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);
    //     uint16_t data = (rx_data[1] << 8) + rx_data[2];

    //     return data;  //< データは2バイト目に格納される
    // }

    void write(const uint8_t address, const uint8_t data) {

        constexpr uint16_t size = 2;
        // const uint8_t write_mode = 0b00000000;  //< (read:1, write:0)
        const uint8_t tx_data[size] = {address, data};

        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_RESET);  // CS pin: LOW
        HAL_SPI_Transmit(&_hspi, (uint8_t*)tx_data, size, 100);
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);  // CS pin: HIGH
    }

    void setConfigs() {
        HAL_Delay(10);
        setPowerManage();
        HAL_Delay(10);
        setAccelConfig();
        HAL_Delay(10);
    }

    // 加速度のレンジの設定
    void setAccelConfig() {
        constexpr uint8_t accel_conf_addr = 0x50;
        constexpr uint8_t accel_conf_data = 0b01100110;  // range:2[g], rate:1k[Hz]
        write(accel_conf_addr, accel_conf_data);
    }

    // 温度・ジャイロ・加速度のON/OFFやモードの設定
    void setPowerManage() {
        constexpr uint8_t accel_conf_addr = 0x4E;
        constexpr uint8_t accel_conf_data = 0b00011111;  // range:2[g], rate:1k[Hz]
        write(accel_conf_addr, accel_conf_data);
    }

    SPI_HandleTypeDef &_hspi;
    GPIO_TypeDef *_cs_x;
    const uint16_t _cs_pin;
};

}  // namespace mslh

#endif //MSLH_GYRO_SENSOR_H
