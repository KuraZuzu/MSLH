/*!
* @file map.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "map.h"

Map::Map(uint8_t x_size, uint8_t y_size) {

    _x_size = x_size;
    _y_size = y_size;
    _block = new Block* [x_size];

    for (int i = 0; i < x_size; ++i) {
        _block[i] = new Block[y_size];    // for(i < x_size)  で  _block[i] = new Block[y_size] では？
    }

    mapInit();

}

Map::~Map() {

    for (int i = 0; i < _y_size; ++i) {
        delete _block[i];
    }
    delete [] _block;
}


void Map::mapInit() {

    _block[0][0].setWall(EAST_MASK + SOUTH_MASK + WEST_MASK);
    _block[1][0].setWall(WEST_MASK);

    for(int i = 0; i < _x_size; i++){
        _block[_x_size - 1][i].setWall(EAST_MASK);
        _block[0][i].setWall(WEST_MASK);
    }

    for(int i = 0; i < _y_size; i++){
        _block[i][0].setWall(SOUTH_MASK);
        _block[i][_y_size - 1].setWall(NORTH_MASK);
    }
}

void Map::setBlock(Block block, Point<uint8_t> point) {
    _block[point._x][point._y].walk_cnt=block.walk_cnt;
    _block[point._x][point._y].setSearched();
    _block[point._x][point._y].setWall(block.getWall());

    if(((block.getWall() & NORTH_MASK) == NORTH_MASK) && (point._y < _y_size - 1)) {
        _block[point._x][point._y + 1].setWall(SOUTH_MASK);
    }
    if(((block.getWall() & WEST_MASK) == WEST_MASK) && (0 < point._x)) {
        _block[point._x - 1][point._y].setWall(EAST_MASK);
    }
    if(((block.getWall() & EAST_MASK) == EAST_MASK) && (point._x < _x_size - 1)) {
        _block[point._x + 1][point._y].setWall(WEST_MASK);
    }
    if(((block.getWall() & SOUTH_MASK) == SOUTH_MASK) && (0 < point._y)) {
        _block[point._x][point._y - 1].setWall(NORTH_MASK);
    }
}

Block& Map::at(Point<uint8_t> point) {
    return _block[point._x][point._y];
}


