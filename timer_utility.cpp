/*!
 * @file timer_utility.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.22
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "timer_utility.h"

uint32_t tim_count = 0;
bool timer_start_flag = false;

void my_timer::wait_timer_start() {
    if(!timer_start_flag) {
//        HAL_TIM_Base_Start_IT(&htim6);
        timer_start_flag = true;
    }
}

void my_timer::wait_timer_stop() {
    if(timer_start_flag) {
//        HAL_TIM_Base_Start_IT(&htim6);
        timer_start_flag = false;
    }
}

void my_timer::wait_us(uint32_t time_us) {
    tim_count = 0;
    while (tim_count < time_us) {
    }
}
