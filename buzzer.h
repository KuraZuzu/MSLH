/**
 * 2021/02/13
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_BUZZER_H
#define ZUZUHALFTPPMOD1_BUZZER_H

#include "tim.h"

/**
 * Sounds the buzzer.
 *
 * Example:
 * @code
 * // Sound the buzzer three times.
 *
 * #include "buzzer.h"
 *
 * Buzzer buzzer(htim8, TIM_CHANNEL_1);
 *
 * int main() {
 *
 *     MX_TIM8_Init();  // Need setup HAL_Timer of PWM
 *
 *     int i = 0;
 *     for(int i = 0; i < 3; i++) {
 *         buzzer.beep(i);
 *     }
 * }
 * @endcode
 */
class Buzzer {

private:
    TIM_HandleTypeDef& _htim_x;
    const uint32_t _channel;

public:

    /**
     * @param Times of beep sound.
     */
    Buzzer(TIM_HandleTypeDef& htim_x, uint32_t channel);

    void beep_x(uint16_t times);
};


#endif //ZUZUHALFTPPMOD1_BUZZER_H
