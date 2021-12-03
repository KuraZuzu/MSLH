/*!
* @file my_timer.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.31
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_TIMER_H
#define ZUZUHALFTPPMOD1_TIMER_H

#include "arm_math.h"
#include "stm32f4xx_hal.h"
#include "tim.h"


namespace timer {

    extern uint32_t us_counter;
//    extern TIM_HandleTypeDef *timer_htim_x;

    /*
     * 1us間隔で呼ばれるcallback関数のTIMをセット
     **/
    void init_timer(TIM_HandleTypeDef *htim_x);

    /*
     * 1us間隔で呼ばれるように対象のTIMの設定をした上でのみ使用可能
     **/
    void wait_us(uint32_t time_us);

    /*
     * 1usで呼ばれるcallback関数内で呼ぶ
     **/
    inline void count_us(){
        us_counter++;
    }

}

#endif //ZUZUHALFTPPMOD1_TIMER_H
