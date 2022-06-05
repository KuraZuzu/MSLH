/*!
* @file map.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/
#ifndef MSLH_MAP_H
#define MSLH_MAP_H

#include "block.h"
#include "defines.h"
#include "point.h"

namespace mslh {

class Map {
public:
    Map(uint8_t x_size, uint8_t y_size);

    Map(const Map& map) {
        Map(map._x_size, map._y_size); // デフォルトコンストラクタ呼び出して、マップのblockポインタを動的確保
        for (uint8_t i = 0; i < _x_size; ++i) {
            for (uint8_t j = 0; j < _y_size; ++j) {
                _block[i][j] = map._block[i][j];
            }
        }
    }

    ~Map();

    /**
     * @fn マップの壁情報とかセット
     */
    void setBlock(Block block, Point<uint8_t> point);

    /**
     * @fn 歩数情報をセット
     */
    void setWalkCount(Point<uint8_t> point, int16_t walk_count){
        _block[point._x][point._y].setWalkCount(walk_count);
    }

    /**
     * @fn 現在地のブロック座標をセット
     */
    Block& at(Point<uint8_t> point);


protected:
    void mapInit();

    Block **_block;
    uint8_t _x_size;
    uint8_t _y_size;

    Point <uint8_t>_point;
};

}  // namespace mslh

#endif //MSLH_MAP_H
