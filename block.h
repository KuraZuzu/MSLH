/*!
* @file block.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef MSLH_BLOCK_H
#define MSLH_BLOCK_H


#include "defines.h"

class Block {

public:
    Block();

    /**
     * @fn センサを参照せずに、マップ情報を元に壁の有無を返す
     * @return (opened = 1), (not opened = 0)
     */
    [[gnu::warn_unused_result]] bool isOpenedWall(uint8_t target_wall) const;

    /**
     * @fn 壁情報を取得する
     */
    [[gnu::warn_unused_result]] uint8_t getWall() const;

    /**
     * @fn 壁情報をセットする
     */
    void setWall(uint8_t wall_data);

    /**
     * @fn 壁情報をリセットする
     */
    void resetWall();

    /**
     * @fn 対象ブロックがゴール座標までの歩数を返す
     */
    [[gnu::warn_unused_result]] int16_t getWalkCount() const;

    /**
     * @fn 対象ブロックがゴール座標までの歩数を設定する
     */
    void setWalkCount(int16_t walk_count);

    /**
     * @fn 対象ブロックが既に探索済みかを返す
     * @return (searched = 1), (not searched = 0)
     */
    bool isSearched();

    /**
     * @fn 対象ブロックが既に探索済みかのフラグをsetする
     * @note (searched = 1), (not searched = 0)
     */
    void setSearched();

private:
    /**
     * @note 0b0000****  <- 下4bitに各方角の壁情報を保存 <br>
     *       NESW (North, East, South, West)//
     */
    uint8_t _block_data;
    int16_t _walk_count; //歩数情報
};


#endif //MSLH_BLOCK_H
