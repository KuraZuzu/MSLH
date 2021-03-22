/*!
* @file Bus3Out.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "arm_math.h"
#include "bus3_out.h"

mslh::Bus3Out::Bus3Out(DigitalOut bit3, DigitalOut bit2, DigitalOut bit1)
        : _bit{bit3, bit2, bit1}, _pinstate(0), _mask(0b00000001){
}

void mslh::Bus3Out::write(int32_t value) {
    _pinstate = value % 8;

    for (uint16_t i = 0; i < 3; ++i) {
        if(_pinstate & (_mask << i)) _bit[i] = 1;
        else _bit[i] = 0;
    }
}


mslh::Bus3Out &mslh::Bus3Out::operator=(int32_t value) {
    _pinstate = value % 8;
    write(_pinstate);
    return *this;
}

mslh::Bus3Out::operator int32_t() const { return _pinstate; }
