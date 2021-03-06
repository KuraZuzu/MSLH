/*!
 * @file distance_sensor.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "buzzer.h"

void Buzzer::beep_x(uint16_t times) {

    for(uint16_t i = 0; i < times; i++) {
        _buzzer_pwm.start();
        HAL_Delay(50);
        _buzzer_pwm.stop();
        HAL_Delay(50);
    }
}