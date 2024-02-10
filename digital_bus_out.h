/*!
* @file DigitalBusOut.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.16
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef DIGITAL_BUS_OUT_H
#define DIGITAL_BUS_OUT_H

#include <vector>
#include "arm_math.h"
#include "digital_out.h"

namespace mslh {

class DigitalBusOut {

public:

    template <typename... DigitalOuts>
    DigitalBusOut(DigitalOuts... digital_outs): _digital_out{digital_outs...} {}

    void write(uint32_t value) {
        size_t i = 0;
        for (auto &digital_out : _digital_out) {
            const int32_t bit = static_cast<int32_t>((value >> i) & 1);
            digital_out.write(bit);
            i++;
        }
    }

    DigitalBusOut(const DigitalBusOut &obj): _digital_out(obj._digital_out) {}

    DigitalBusOut &operator= (uint32_t value) {
        write(value);
        return *this;
    }

    /**
     * @fn
     *   GPIO_Pinstate (pin output) can be treated like a variable.  <br>
     *   ピン出力を変数のように扱えるようになるが，バグの温床となる可能性もある．
     */
    operator uint32_t () const {
        uint32_t value = 0;

        size_t i = 0;
        for (auto &digital_out : _digital_out) {
            const int32_t bit = (digital_out._pinstate == 0) ? 0 : 1;
            value += static_cast<int32_t>(bit << i);
            i++;
        }

        return value;
    }

private:

    std::vector<DigitalOut> _digital_out;
};

}  // namespace mslh

#endif  // DIGITAL_BUS_OUT_H
