//
// Created by 倉澤　一詩 on 2019/10/26.
//

#ifndef ZUZUHALFTPPMOD1_USART_EXPAND_H
#define ZUZUHALFTPPMOD1_USART_EXPAND_H

// Math parameters
constexpr PI = 3.141592;

// Machine speed parameters
constexpr ACCEL = 0.05;

// Unit is "pulse"
constexpr MES6_x4_PULSE = 500*4;

// Unit is "mm"
constexpr WHEEL_DIAMETER = 13.5;
constexpr ONE_BLOCK = 90;
constexpr MACHINE_TREAD = 36.2; /*(40 - 3.8)*/
constexpr DISTANCE_PER_PULSE = 2 * WHEEL_DIAMETER * PI / MES6_x4_PULSE;


#endif //ZUZU_HALFMOUSE_2019_PARAMETER_H
