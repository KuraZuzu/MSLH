/*!
* @file Point.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef MSLH_POINT_H
#define MSLH_POINT_H

#include "defines.h"
#include "arm_math.h"
//#include <cmath>

namespace mslh {

template <typename T>
struct Point{

public:
    Point(): _x(0), _y(0) {}
    Point(T x, T y): _x(x), _y(y) {}

    T _x;
    T _y;
};


struct Position : public Point<float32_t>{

public:
    Position();

    Position(const Position& position);

    Position operator - (const Position &position);

    Position operator + (const Position &position);

    Position operator / (const Position& position);

    Position operator * (const Position& position);


    float32_t _rad;
};


struct MapPosition : public Point<uint8_t> {

public:
    MapPosition():_direction(0.0) {};

    MapPosition(const MapPosition& map_position);

    bool operator == (const MapPosition& position)const;

    bool operator != (const MapPosition& position)const;


    uint8_t _direction;
};

}  // namespace mslh

#endif //MSLH_POINT_H
