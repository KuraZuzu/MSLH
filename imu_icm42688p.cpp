/*!
 * @file gyro_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.07
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "imu_icm42688p.h"


// for DMA
// void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

//     if(hspi->Instance == SPI3) {
//         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);  // CS pin : HIGH
//     }
// }