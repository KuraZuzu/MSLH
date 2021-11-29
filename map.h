/*!
* @file map.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/
#ifndef ZUZUHALFTPPMOD1_MAP_H
#define ZUZUHALFTPPMOD1_MAP_H

#include "block.h"
#include "defines.h"
#include "point.h"

class Map {
public:
    Map(uint8_t x_size, uint8_t y_size);

    Map(const Map& map){
        Map(map._x_size, map._y_size);
        for (int i = 0; i < _x_size; ++i) {
            for (int j = 0; j < _y_size; ++j) {
                _block[i][j] = map._block[i][j];
            }
        }
    }

    ~Map();

/* マップの壁情報とかセット */
    void setBlock(Block block, Point<uint8_t> point);

/* 歩数情報をセット */
    void setWalkCount(Point<uint8_t> point, int walk_cnt){
        _block[point._x][point._y].walk_cnt = walk_cnt;
    }

    Block& at(Point<uint8_t> point);

    Point<uint8_t> size(){
        return Point<uint8_t >(_x_size, _y_size);
    }


protected:
    void mapInit();

    Block **_block;
    uint8_t _x_size = 0;
    uint8_t _y_size = 0;

    Point <uint8_t>_point;
};


#endif //ZUZUHALFTPPMOD1__MAP_H
