/*!
* @file map.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.11.28
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_MAP_H
#define ZUZUHALFTPPMOD1_MAP_H

#include "defines.h"
#include <vector>

struct Block {
    uint8_t _wall_data = 0;
    uint8_t _walk_count = UINT8_MAX;
};

class Map {

public:
    Map(uint8_t map_size_x, uint8_t map_size_y) {

        _block.resize(map_size_x);

        for (int i = 0; i < map_size_y; ++i) {
            _block[i].resize(map_size_y);
        }

    }

    ~Map() {}

    void set_wall(uint8_t x, uint8_t y, uint8_t wall_data) {
    }

    void update_walk_count() {
    }

    void set_goal(uint8_t goal_x, uint8_t goal_y) {
        _goal_x = goal_x;
        _goal_y = goal_y;
    }

private:
    uint8_t _goal_x = 8;
    uint8_t _goal_y = 8;
    std::vector< std::vector<Block> > _block;
};


#endif //ZUZUHALFTPPMOD1_MAP_H
