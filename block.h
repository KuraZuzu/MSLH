/*!
* @file block.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2018.11.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef NEWZUZUMOUSE_BLOCK_H
#define NEWZUZUMOUSE_BLOCK_H


#include "defines.h"

class Block {

private:

    uint8_t _b;  /* 0b0000****  <- 下4bitに各方角の壁情報を保存
                          NESW (North, East, South, West)//  */

public:
    Block();

    int walk_cnt; //歩数情報

    /**
     * @fn 壁情報をセットする
     */
    void set_wall(uint8_t b);

    /**
     * @fn 壁情報をリセットする
     */
    void reset_wall();

    /**
     * @fn センサを参照せずに、マップ情報を元に壁の有無を返す
     */
    bool is_opened_wall(uint8_t t);

    /**
     * @fn 壁情報を取得する
     */
    uint8_t get_wall();

    void set_searched();

    bool is_searched();
};


#endif //NEWZUZUMOUSE_BLOCK_H
