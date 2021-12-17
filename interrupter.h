/*!
* @file block.h v1.0
* @Copyright © 2018 Kazushi Kurasawa
* @date 2021.12.18
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef MSLH_INTERRUPTER_H
#define MSLH_INTERRUPTER_H

#include "vector.h"

namespace mslh {


    template<class T>
    class Interrupter {

    public:
        void attach(T *obj, void (T::*func)()) {
            _obj = obj;
            _func = func;
        }

        void run() {
            (_obj->*_func)();
        }

    private:
        T* _obj;
        void (T::*_func)();

    };


}

#endif //MSLH_INTERRUPTER_H
