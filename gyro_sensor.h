/*!
 * @file gyro_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.07
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "spi.h"
#include "digital_out.h"

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

private:
    SPI_HandleTypeDef& _hspi;
    DigitalOut _cs_pin;
};


#endif //ZUZUHALFTPPMOD1_GYRO_SENSOR_H
