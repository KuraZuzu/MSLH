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
        uint32_t us_counter;
        TIM_HandleTypeDef *timer_htim_x;
};
//uint32_t timer::us_counter;
//TIM_HandleTypeDef *timer::timer_htim_x;

/**
 * @fn init_timer_setting(TIM)
 */
void timer::init_timer(TIM_HandleTypeDef *htim_x) {
    timer_htim_x = htim_x;
    HAL_TIM_Base_Start_IT(timer_htim_x);
}

/**
 * @fn timer wait function.
 */
void timer::wait_us(uint32_t time_us) {
    us_counter = 0;
    HAL_TIM_Base_Start_IT(timer_htim_x);
    while (us_counter < time_us) {
    }
    HAL_TIM_Base_Stop_IT(timer_htim_x);
}
