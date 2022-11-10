/*!
* @file my_timer.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.31
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef MSLH_TIMER_H
#define MSLH_TIMER_H

#include "arm_math.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

/**
 * 1usごとのタイマーを用いる必要あり
 */
class Timer {

public:
    Timer(TIM_HandleTypeDef &htim_x): _htim_x(htim_x), time(0) {}

    inline uint32_t getTime() { return time; }
    inline void interruptTimerCount() { time++; }
    inline void start() { HAL_TIM_Base_Start(&_htim_x); } //Start It の必要あるか調査
    inline void stop() { HAL_TIM_Base_Stop(&_htim_x); }
    void reset() { time = 0; }

private:
    uint32_t time;
    TIM_HandleTypeDef &_htim_x;
};

namespace timer {

    extern volatile uint32_t counter_us;
    extern TIM_HandleTypeDef *timer_htim_x;

    /*
     * 1us間隔で呼ばれるcallback関数のTIMをセット
     **/
    void startTimer(TIM_HandleTypeDef *htim_x);

    /*
     * 1us間隔で呼ばれるcallback関数のTIMをセット
     **/
    void stopTimer(TIM_HandleTypeDef *htim_x);

    /*
     * 1us間隔で呼ばれるように対象のTIMの設定をした上でのみ使用可能
     **/
    void waitMicroSeconds(uint32_t time_us);

    /*
     * 1us間隔で呼ばれるcallback関数内で呼ぶ
     **/
    inline void countMicroSeconds(){
        counter_us++;
    }

}

#endif //MSLH_TIMER_H
