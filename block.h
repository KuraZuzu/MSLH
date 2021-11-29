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

    int walk_cnt; //歩数情報

    /**
     * @fn 壁情報をセットする
     */
    void setWall(uint8_t b);

    /**
     * @fn 壁情報をリセットする
     */
    void resetWall();

    /**
     * @fn センサを参照せずに、マップ情報を元に壁の有無を返す
     */
    bool isOpenedwall(uint8_t t);

    /**
     * @fn 壁情報を取得する
     */
    uint8_t getWall();

    void setSearched();

    bool isSearched();
};


#endif //ZUZUHALFTPPMOD1_BLOCK_H
