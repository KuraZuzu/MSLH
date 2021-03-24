/*!
 * @file defines.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_DEFINES_H
#define ZUZUHALFTPPMOD1_DEFINES_H

#include <cstdio>
#include "usart.h"
#include "arm_fp16.h"
#include "arm_math.h"

//void delay_us() {
//    asm("NOP");
//}

//namespace timer {}

namespace timer {
//  static int abs_time = 3;

  static void timerStart() {
      RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
      TIM6->PSC = 9999;
      TIM6->CNT = 0;
      TIM6->CR1 = 0;

      TIM6->DIER = TIM_DIER_UIE;
      NVIC_EnableIRQ(TIM6_DAC_IRQn);
  };
}

#endif //ZUZUHALFTPPMOD1_DEFINES_H
