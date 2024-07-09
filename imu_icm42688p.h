/*!
 * @file gyro_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.07
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_IMU_H
#define MSLH_IMU_H

#include "arm_math.h"
#include "spi.h"
#include "digital_out.h"

namespace mslh {

class IMUICM42688P {

public:
    IMUICM42688P(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs_x, uint16_t cs_pin)
    : _hspi(hspi)
    , _cs_x(cs_x)
    , _cs_pin(cs_pin)
    , _accel_range(2.0f)
    , _gyro_range(2000.0f)
    ,  _raw_angular_velocity_z_offset(0) {
    }

    void init() {
        HAL_SPI_Init(&_hspi);
        HAL_GPIO_WritePin(_cs_x, _cs_pin, GPIO_PIN_SET);
        setConfigs();
    }

    /* ジャイロの零点ドリフト補正 */
    void calibrate() {
        HAL_Delay(3000);
        int32_t tmp_value = 0;
        for (int i = 0; i < 1000; i++) tmp_value += static_cast<int32_t>(static_cast<int16_t>(((read(0x29) << 8) | read(0x2A))));
        _raw_angular_velocity_z_offset = tmp_value / 1000;
    }

    uint8_t getWhoAmI() {
        const uint8_t address = 0x75;   //< Input who_am_i: 0x75, return: 0x47
        return read(address);  // 受け取るのは2バイト目のデータだけ
    }

    float32_t getAngularVelocityZ() {
        // 0x29: upper Byte,  0x2A: lower Byte
        const int16_t raw_angular_velocity_z = static_cast<int16_t>(((read(0x29) << 8) | read(0x2A)));
        return (_gyro_range* static_cast<float32_t>(raw_angular_velocity_z - _raw_angular_velocity_z_offset) / static_cast<float32_t>(INT16_MAX));
    }

    float32_t getAccelX() {
        // 0x1F: upper Byte,  0x20: lower Byte
        const int16_t raw_accel_x = static_cast<int16_t>(((read(0x1F) << 8) | read(0x20)));
        return (_accel_range* static_cast<float32_t>(raw_accel_x) / static_cast<float32_t>(INT16_MAX));
    }

    float32_t getAccelY() {
        // 0x21: upper Byte,  0x22: lower Byte
        const int16_t raw_accel_y =  static_cast<int16_t>((read(0x21) << 8) | read(0x22));
        return (_accel_range * static_cast<float32_t>(raw_accel_y) / static_cast<float32_t>(INT16_MAX));
    }

    float32_t getAccelZ() {
        // 0x23: upper Byte,  0x24: lower Byte
        int16_t raw_accel_z = static_cast<int16_t>((read(0x23) << 8) | read(0x24));
        return (_accel_range * static_cast<float32_t>(raw_accel_z) / static_cast<float32_t>(INT16_MAX));
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
        setGyroConfig();
        HAL_Delay(10);
        setAccelConfig();
        HAL_Delay(10);
    }

    void setGyroConfig() {
        constexpr uint8_t gyro_conf_addr = 0x4F;
        constexpr uint8_t gyro_conf_data = 0b00000110;  // range:1000[deg/s], rate: 1k[Hz]
        // constexpr uint8_t gyro_conf_data = 0b01100110;  // range:250[deg/s], rate: 1k[Hz]
        write(gyro_conf_addr, gyro_conf_data);
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
        constexpr uint8_t accel_conf_data = 0b00011111;  // [gyro][accel]:: Low Noise mode
        write(accel_conf_addr, accel_conf_data);
    }


    SPI_HandleTypeDef &_hspi;
    GPIO_TypeDef *_cs_x;
    const uint16_t _cs_pin;
    float32_t _accel_range;  // default: 2.0f
    float32_t _gyro_range;  // default 2000[deg]/s
    int16_t _raw_angular_velocity_z_offset;
};

}  // namespace mslh

#endif //MSLH_IMU_H