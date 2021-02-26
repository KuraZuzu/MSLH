/**
 * 2021/02/20
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_SERIAL_UTILITY_H
#define ZUZUHALFTPPMOD1_SERIAL_UTILITY_H

extern "C" {
#include "usart.h"
}
#include <cstdio>

/** 9600 Port-Rate is recommended */
class SerialUtility {

public:
    SerialUtility(UART_HandleTypeDef* uart_handle):_uart_handle(uart_handle){
    }

    void get_print(){
        std::printf("test test\n");
    }



private:
    UART_HandleTypeDef* _uart_handle;

};


#endif //ZUZUHALFTPPMOD1_SERIAL_UTILITY_H
