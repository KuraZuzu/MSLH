/*!
 * @file timer_utility.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.22
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_TIMER_UTILITY_H
#define ZUZUHALFTPPMOD1_TIMER_UTILITY_H

#include "arm_math.h"
#include "tim.h"
#include "stm32f4xx_it.h"


extern uint32_t tim_count;
extern bool timer_start_flag;

namespace my_timer {

//    extern uint32_t tim_count;
//    extern bool timer_start_flag;

    void wait_timer_start();
    void wait_timer_stop();

    void wait_us(uint32_t time_us);


}  //< namespace my_timer


#endif //ZUZUHALFTPPMOD1_TIMER_UTILITY_H
