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
#include <cmath>

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
    Position(): _rad(0.0){
        _x = 0.0;
        _y = 0.0;
    }

    Position(const Position& position): Point(position), _rad(position._rad) {
        _x = position._x;
        _y = position._y;
    }

    Position operator - (const Position &position) {
        Position temp_position;
        temp_position._x = _x - position._x;
        temp_position._y = _y - position._y;
        temp_position._rad = _rad - position._rad;
        return temp_position;
    }

    Position operator + (const Position &position) {
        Position temp_position;
        temp_position._x = _x + position._x;
        temp_position._y = _y + position._y;
        temp_position._rad = _rad + position._rad;
        return temp_position;
    }

    Position operator / (const Position& position) {
        Position temp_position;
        temp_position._x = _x / position._x;
        temp_position._y = _y / position._y;
        temp_position._rad = _rad / position._rad;
        return temp_position;
    }

    Position operator * (const Position& position) {
        Position temp_position;
        temp_position._x = _x * position._x;
        temp_position._y = _y * position._y;
        temp_position._rad = _rad * position._rad;
        return temp_position;
    }

    float32_t _rad;
};


struct MapPosition : public Point<uint8_t> {

public:
    MapPosition():_direction(0.0) {};

    MapPosition(const MapPosition& map_position): Point(map_position), _direction(0.0f) {
        _x = map_position._x;
        _y = map_position._y;
    }
//    explicit operator Point<uint8_t>() {
//        Point<uint8_t> point;
//        point._x = _x;
//        point._y = _y;
//        return point;
//    }

    bool operator == (const MapPosition& position)const {
        return ((_x == position._x) && (_y == position._y));
    }

    bool operator != (const MapPosition& position)const {
        return !operator == (position);
    }

    uint8_t _direction;
};

#endif //MSLH_POINT_H
