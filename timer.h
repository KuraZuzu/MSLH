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
 * @brief
 *   1[us]単位で時間を計測して遅延処理と任意のタイミングで時間取得を行う
 *
 * @note
 *   TIMのカウント数をアドレスで保存して参照しているため，同一のTIMを用いて複数のインスタンスを生成しても問題ない．
 *
 * @warning
 *   ・1usとなるようなプリスケーラを用いる必要あり
 *     ("Clock Configuration" タブから使用するTIMの "APBn Timer clocks (MHz)") / (使用するTIMの "Prescaler") = 1,000,000
 *     となるように設定が必要
 *   ・"Counter Period (AutoReload Register)" はカウントできる最大値となるため，極力大きい数値とすることを推奨する．
 *   ・"auto-reload preload" は "Enable" とすること．
 *   ・"Nvic Setting" の "TIMn global interrupt" にチェックは入れない．
 *
 * @example
 *   TIM2を使用する場合は"APB1"なので，"Clock Configuration"タブの"APB1 Timer Clocks(MHz)"の欄を見る．(今回は 90MHz とする)
 *   だとしたらTIM2の設定は "Prescaler = 90-1" となる  (90[MHz] / 90 = 1[us])．
 *   "Prescaler = 90-1" としているのは，0の値も１分周器となるため，90回だと "90 - 1 = 89" とする必要があるため．
 */
namespace mslh {

    class Timer {

    public:
        Timer(TIM_HandleTypeDef &timer_htim_n) : _timer_htim_n(timer_htim_n) {}

        inline void start() { HAL_TIM_Base_Start(&_timer_htim_n);}

        inline void stop() { HAL_TIM_Base_Stop(&_timer_htim_n); }

        inline uint32_t getTime() const { return _timer_htim_n.Instance->CNT; }

        inline void waitMicroSeconds(uint32_t micro_seconds) const {
            const uint32_t offset_time = _timer_htim_n.Instance->CNT;
            while (_timer_htim_n.Instance->CNT - offset_time < micro_seconds) {}
        }


    private:
        TIM_HandleTypeDef &_timer_htim_n;

    };

}

#endif //MSLH_TIMER_H
