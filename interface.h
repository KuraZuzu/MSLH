//
// Created by 倉澤　一詩 on 2021/03/20.
//

#ifndef ZUZUHALFTPPMOD1_INTERFACE_H
#define ZUZUHALFTPPMOD1_INTERFACE_H

#include "stm32f4xx_hal.h"

class Interface {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif //ZUZUHALFTPPMOD1_INTERFACE_H