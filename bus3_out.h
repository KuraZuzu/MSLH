/*!
* @file Bus3Out.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_BUS3_OUT_H
#define ZUZUHALFTPPMOD1_BUS3_OUT_H

#include "digital_out.h"

class Bus3Out {

public:

    Bus3Out(DigitalOut bit3, DigitalOut bit2, DigitalOut bit1);

    void write(int32_t value);

    Bus3Out& operator= (int32_t value);

    /**
     * @fn
     *   GPIO_Pinstate (pin output) can be treated like a variable.  <br>
     *   ピン出力を変数のように扱えるようになるが，バグの温床となる可能性もある．
     */
    operator int32_t () const;

private:
    const uint8_t _mask;
    DigitalOut _bit[3];
    int32_t _pinstate;
};


#endif //ZUZUHALFTPPMOD1_BUS3_OUT_H
