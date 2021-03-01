/**
 * 2019/10/26
 * @author KuraZuzu
 */

#include <cstdint>

#ifndef ZUZUHALFTPPMOD1_SERIAL_UTILITY_H
#define ZUZUHALFTPPMOD1_USART_EXPAND_H

// Math parameters
constexpr double PI = 3.141592;

// Machine speed parameters
constexpr double ACCEL = 0.05;

// Unit is "pulse"
constexpr uint16_t MES6_x4_PULSE = 500*4;

// Unit is "mm"
constexpr double WHEEL_DIAMETER = 13.5;
constexpr uint16_t ONE_BLOCK = 90;
constexpr double MACHINE_TREAD = 36.2; /*(40 - 3.8)*/
constexpr double DISTANCE_PER_PULSE = 2 * WHEEL_DIAMETER * PI / MES6_x4_PULSE;


#endif //ZUZU_HALFMOUSE_2019_PARAMETER_H
