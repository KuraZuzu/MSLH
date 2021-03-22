/*!
 * @file timer_utility.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.22
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "timer_utility.h"

void my_timer::wait_timer_start() {
    if(!timer_start_flag) {
        HAL_TIM_Base_Start_IT(&htim6);
        timer_start_flag = true;
    }

}