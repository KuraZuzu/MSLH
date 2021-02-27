/**
 * 2021/02/20
 *
 * This class is no implemented.
 *
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_SERIAL_UTILITY_H
#define ZUZUHALFTPPMOD1_SERIAL_UTILITY_H

#include "usart.h"
#include "stdio.h"
//#include <cstdio>
//#include <cstdarg>


/** Portraits below 9600 are recommended
 *  huart_X.Init.BaudRate = 9600; */
class SerialUtility {

public:
    UART_HandleTypeDef* _huart;

    SerialUtility(UART_HandleTypeDef* huart):_huart(huart){
        setbuf(stdout, NULL);
    }

};


#endif //ZUZUHALFTPPMOD1_SERIAL_UTILITY_H
