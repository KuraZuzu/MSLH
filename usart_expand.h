/**
 * 2021/02/20
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_USART_EXPAND_H
#define ZUZUHALFTPPMOD1_USART_EXPAND_H

extern "C" {
#include "usart.h"
}
#include <cstdio>

/** default portrate is 115200 */
class UsartExpand {

public:
    UsartExpand(UART_HandleTypeDef* uart_handle):_uart_handle(uart_handle){
    }

    void get_print(){
        std::printf("test test\n");
    }



private:
    UART_HandleTypeDef* _uart_handle;

};


#endif //ZUZUHALFTPPMOD1_USART_EXPAND_H
