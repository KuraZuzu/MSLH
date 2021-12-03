/*!
* @file my_timer.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.31
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "timer.h"

namespace timer {
        uint32_t counter_us;
        TIM_HandleTypeDef *timer_htim_x;
};
//uint32_t timer::counter_us;
//TIM_HandleTypeDef *timer::timer_htim_x;

/**
 * @fn init_timer_setting(TIM)
 */
void timer::initTimer(TIM_HandleTypeDef *htim_x) {
    timer_htim_x = htim_x;
    HAL_TIM_Base_Start_IT(timer_htim_x);
}

/**
 * @fn timer wait function.
 */
void timer::waitMicroSeconds(uint32_t time_us) {
    counter_us = 0;
    HAL_TIM_Base_Start_IT(timer_htim_x);
    while (counter_us < time_us) {
    }
    HAL_TIM_Base_Stop_IT(timer_htim_x);
}
