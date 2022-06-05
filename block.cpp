/*!
* @file block.cpp v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "block.h"

Block::Block() {
    _block_data = 0b00000000;
    _walk_count = 255;
}

bool Block::isOpenedWall(uint8_t target_wall) const { return (_block_data & target_wall); }

uint8_t Block::getWall() const { return 0b1111 & _block_data; }

void Block::setWall(uint8_t wall_data) { _block_data |= wall_data; }

void Block::resetWall() { _block_data = 0b00000000; }

int16_t Block::getWalkCount() const { return _walk_count; }

void Block::setWalkCount(int16_t walk_count) { _walk_count = walk_count; }

bool Block::isSearched() { return (_block_data & SEARCH_MASK) == SEARCH_MASK; }

void Block::setSearched() { _block_data |= SEARCH_MASK; }