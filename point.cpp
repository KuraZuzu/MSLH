/*!
* @file Point.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "point.h"

mslh::Position::Position() : _rad(0.0f){
    _x = 0.0;
    _y = 0.0;
}

mslh::Position::Position(const mslh::Position &position) : Point(position), _rad(position._rad) {
    _x = position._x;
    _y = position._y;
}

void mslh::Position::reset() {
    _x = 0.0f;
    _y = 0.0f;
    _rad = 0.0f;
}

mslh::Position mslh::Position::operator-(const mslh::Position &position) {
    Position temp_position;
    temp_position._x = _x - position._x;
    temp_position._y = _y - position._y;
    temp_position._rad = _rad - position._rad;
    return temp_position;
}

mslh::Position mslh::Position::operator+(const mslh::Position &position) {
    Position temp_position;
    temp_position._x = _x + position._x;
    temp_position._y = _y + position._y;
    temp_position._rad = _rad + position._rad;
    return temp_position;
}

mslh::Position mslh::Position::operator/(const mslh::Position &position) {
    Position temp_position;
    temp_position._x = _x / position._x;
    temp_position._y = _y / position._y;
    temp_position._rad = _rad / position._rad;
    return temp_position;
}

mslh::Position mslh::Position::operator*(const mslh::Position &position) {
    Position temp_position;
    temp_position._x = _x * position._x;
    temp_position._y = _y * position._y;
    temp_position._rad = _rad * position._rad;
    return temp_position;
}


mslh::MapPosition::MapPosition(const mslh::MapPosition &map_position) : Point(map_position), _direction(0.0f) {
    _x = map_position._x;
    _y = map_position._y;
}

bool mslh::MapPosition::operator==(const mslh::MapPosition &position) const {
    return ((_x == position._x) && (_y == position._y));
}

bool mslh::MapPosition::operator!=(const mslh::MapPosition &position) const {
    return !operator == (position);
}
