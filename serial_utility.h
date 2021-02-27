/**
 * 2021/02/20
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
    SerialUtility(UART_HandleTypeDef* huart):_huart(huart){
//        setbuf(stdout, NULL);
    }

//    int
//    __printf (const char *format, ...){
//        va_list arg;
//        int done;
//        va_start (arg, format);
//        done = vfprintf (stdout, format, arg);
//        va_end (arg);
//
//        return done;
//    }

private:
    UART_HandleTypeDef* _huart;

};


#endif //ZUZUHALFTPPMOD1_SERIAL_UTILITY_H
