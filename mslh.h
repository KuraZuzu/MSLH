/*!
 * @file mslh.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.22
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

/**
 * @warning
 *     TIM6はユーザー側での使用もしてないけません．
 *     TimerUtilityクラスのために以下の通りにTIM6の設定を行ってください．
 *     設定する項目は以下の3点です．
 *
 *     ・ TIM6に使われるTIM(STM32F446ReT6 では "APB1")の周波数   _____(1)
 *     ・ Prescaler (PSC - 16 bits value)                 　　 ____(2)
 *     ・ Counter Perio (AutoReload Register - 16 bits value)　 ___(3)
 *
 *     これらを元にした以下の計算式の結果が 10^-6 となるように設定してください．
 *
 *         (1) / ((2) + 1) / ((3) + 1) = 10^-6
 *
 *
 * @Example
 *     (1) が 90 M[Hz] だった場合．
 *
 *     90 M[Hz] / ((90-1) + 1) / ((1-1) + 1)  =  10^-6
 *      (1)            (2)            (3)
 *
 *
 *      よって，
 *      (1) 90 M[Hz]
 *      (2) 90-1
 *      (3) 1-1
 */

#ifndef MSLH_MSLH_H
#define MSLH_MSLH_H

#include "parameter.h"
#include "analog_in_dma_stream.h"
#include "distance_sensor.h"
#include "digital_out.h"
#include "bus3_out.h"
#include "pwm_out.h"
#include "buzzer.h"
#include "motor.h"
#include "encoder.h"
#include "wheel.h"
#include "gyro_sensor.h"
#include "wheel.h"
#include "gpio_distance_sensor.h"
#include "interrupter.h"
#include "point.h"
#include "map.h"
#include "position_estimator.h"


using namespace mslh;

#endif //MSLH_MSLH_H
