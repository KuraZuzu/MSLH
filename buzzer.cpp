/**
 * @date 2021/02/13
 * @author KuraZuzu
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