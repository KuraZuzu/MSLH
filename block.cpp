/*!
* @file block.cpp v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "block.h"

// open = true 1
// close = false 0

void Block::setWall(uint8_t b) {
    _block_data |= b;
}

bool Block::isOpenedwall(uint8_t t) {
    return (_block_data & t);
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
    walk_cnt = 255;
}

