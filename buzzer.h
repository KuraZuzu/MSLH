/**
 * @date 2021.02.13
 * @author Kazushi Kurasawa
 */

#ifndef ZUZUHALFTPPMOD1_BUZZER_H
#define ZUZUHALFTPPMOD1_BUZZER_H

#include "pwm_out.h"

/**
 * @brief
 *   Sounds the buzzer.
 *
 * Example:
 * @code
 *   // Sound the buzzer three times.
 *
 *   #include "buzzer.h"
 *
 *   Buzzer buzzer(htim8, TIM_CHANNEL_1);
 *
 *   int main() {
 *      // Abbreviation Microcomputer startup settings
 *
 *       MX_TIM8_Init();  // Need setup HAL_Timer of PWM
 *
 *       int i = 0;
 *       for(int i = 0; i < 3; i++) {
 *           buzzer.beep(i);
 *       }
 *   }
 * @endcode
 */
class Buzzer : PWMOut{

public:

    Buzzer(TIM_HandleTypeDef& htim_x, uint32_t channel)
    : PWMOut(htim_x, channel) {
        write(0.5);
    }

//    Buzzer(PWMOut buzzer_pwm)
//    : _buzzer_pwm(buzzer_pwm){
//        _buzzer_pwm = 0.5;
//    }

    /**
     * @fn Beeps a specified number of times.
     * @param Times of beep sound.
     */
    void beep_x(uint16_t times);


private:
//    PWMOut _buzzer_pwm;
};


#endif //ZUZUHALFTPPMOD1_BUZZER_H
