/*!
* @file block.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_BLOCK_H
#define ZUZUHALFTPPMOD1_BLOCK_H


#include "defines.h"

class Block {

private:

    uint8_t _block_data;  /* 0b0000****  <- 下4bitに各方角の壁情報を保存
                          NESW (North, East, South, West)//  */

public:
    Block();

    /**
     * @fn 壁情報をセットする
     */
    void setWall(uint8_t wall_data);

    /**
     * @fn 壁情報をリセットする
     */
    void resetWall();

    /**
     * @fn センサを参照せずに、マップ情報を元に壁の有無を返す
     * @return (opened = 1), (not opened = 0)
     */
    bool isOpenedWall(uint8_t target_wall);

    /**
     * @fn 壁情報を取得する
     */
    uint8_t getWall();

    void setSearched();

    /**
     * @fn 対象ブロックが既に探索済みかを返す
     * @return (searched = 1), (not searched = 0)
     */
    bool isSearched();

    int16_t walk_count; //歩数情報
};


#endif //ZUZUHALFTPPMOD1_BLOCK_H
