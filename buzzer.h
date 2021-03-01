/**
 * 2021/02/13
 * @author KuraZuzu
 */

#ifndef ZUZUHALFTPPMOD1_BUZZER_H
#define ZUZUHALFTPPMOD1_BUZZER_H

#include "tim.h"

class Buzzer {

private:
    TIM_HandleTypeDef* _htim_x;
    uint32_t _channel;

public:

    /**
     * @param An argument (uint16_t times) is times of beep sound.
     */
    Buzzer(TIM_HandleTypeDef* htim_x, uint32_t channel);

    void beep_x(uint16_t times);
};


#endif //ZUZUHALFTPPMOD1_BUZZER_H
