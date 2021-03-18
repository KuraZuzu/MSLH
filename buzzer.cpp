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

void Buzzer::error_v1() {
    uint16_t time = 1000;
    for (int i = 0; i < 10; ++i) {
        _buzzer_pwm.start();
        HAL_Delay(time);
        _buzzer_pwm.stop();
        HAL_Delay(50);
        time *= 0.55;
    }
}

void Buzzer::error_v2() {
    _buzzer_pwm.start();
    HAL_Delay(5000);
    _buzzer_pwm.stop();
}
