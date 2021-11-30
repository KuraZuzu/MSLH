/*!
* @file block.cpp v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "block.h"


void Block::setWall(uint8_t wall_data) {
    _block_data |= wall_data;
}

bool Block::isOpenedWall(uint8_t target_wall) {
    return (_block_data & target_wall);
}

void Block::resetWall() {
    _block_data = 0b00000000;
}

uint8_t Block::getWall() {
    return 0b1111 & _block_data;
}

void Block::setSearched() {
    _block_data |= SEARCH_MASK;
}

bool Block::isSearched() {
    return (_block_data & SEARCH_MASK) == SEARCH_MASK;
}

Block::Block() {
    _block_data = 0b00000000;
    walk_count = 255;
}

