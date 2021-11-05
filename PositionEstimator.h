//
// Created by 倉澤　一詩 on 2021/10/03.
//

#ifndef ZUZUHALFTPPMOD1_POSITIONESTIMATOR_H
#define ZUZUHALFTPPMOD1_POSITIONESTIMATOR_H

#include "arm_math.h"

class Position {
public:
    Position();
    float32_t x;
    float32_t y;
    float32_t direction_rad;
};

class PositionEstimator {
public:
    PositionEstimator();

};


#endif //ZUZUHALFTPPMOD1_POSITIONESTIMATOR_H
