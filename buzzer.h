//
// Created by 倉澤　一詩 on 2021/02/13.
//

#ifndef ZUZUHALFTPPMOD1_BUZZER_H
#define ZUZUHALFTPPMOD1_BUZZER_H

extern "C"{
#include "tim.h"
}

class Buzzer {

private:
    TIM_HandleTypeDef* _htim_x;
    uint32_t _channel;

public:
    Buzzer(TIM_HandleTypeDef* htim_x, uint32_t channel);

    void pi();

    void pipi();

    void pipipi();
};


#endif //ZUZUHALFTPPMOD1_BUZZER_H
