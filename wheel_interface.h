//
// Created by 倉澤　一詩 on 2021/03/20.
//

#ifndef ZUZUHALFTPPMOD1_WHEEL_INTERFACE_H
#define ZUZUHALFTPPMOD1_WHEEL_INTERFACE_H

#include "stm32f4xx_hal.h"

class WheelInterface {

public:

    virtual void start();

    virtual void stop();

    virtual void update();
};

#endif //ZUZUHALFTPPMOD1_WHEEL_INTERFACE_H