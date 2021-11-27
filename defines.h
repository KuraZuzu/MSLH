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
//#include "arm_fp16.h" // これ何 ?
#include "arm_math.h"

constexpr uint8_t SEARCH_MASK = 0b00010000;
constexpr uint8_t NORTH_MASK = 0b00001000;
constexpr uint8_t EAST_MASK = 0b00000100;
constexpr uint8_t SOUTH_MASK = 0b00000010;
constexpr uint8_t WEST_MASK = 0b00000001;

#endif //ZUZUHALFTPPMOD1_DEFINES_H
