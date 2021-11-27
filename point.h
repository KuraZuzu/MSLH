/*!
* @file Point.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef NEWZUZUMOUSE_POINT_H
#define NEWZUZUMOUSE_POINT_H

#include <cmath>
#include <inttypes.h>

template <typename T>
struct Point{

public:
    Point():x(0),y(0){}
    Point(T _x, T _y):x(_x),y(_y){}

    T x;
    T y;

};


struct Position : public Point<double_t>{
public:
    Position():rad(0.0){
        x = 0.0;
        y = 0.0;
    }

    Position(const Position& _p): Point(), rad(_p.rad) {
        x = _p.x;
        y = _p.y;
    }

    double_t rad;

    Position operator -(const Position& p){
        Position v;
        v.x = x - p.x;
        v.y = y - p.y;
        v.rad = rad - p.rad;
        return v;
    }

    Position operator +(const Position& p){
        Position v;
        v.x = x + p.x;
        v.y = y + p.y;
        v.rad = rad + p.rad;
        return v;
    }

    Position operator /(const Position& p){
        Position v;
        v.x = x / p.x;
        v.y = y / p.y;
        v.rad = rad / p.rad;
        return v;
    }

    Position operator *(const Position& p){
        Position v;
        v.x = x * p.x;
        v.y = y * p.y;
        v.rad = rad * p.rad;
        return v;
    }


};

struct MapPosition : public Point<uint8_t>{

public:
    uint8_t direction;


    explicit operator Point<uint8_t>(){
        Point<uint8_t> p;
        p.x = x;
        p.y = y;
        return p;
    }

    bool operator==(const MapPosition& _p)const{
        return ((x ==_p.x) && (y == _p.y));
    }

    bool operator!=(const MapPosition& _p)const{
        return !operator==(_p);
    }

};

#endif //NEWZUZUMOUSE_POINT_H
