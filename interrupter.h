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

#include "arm_math.h"

namespace mslh {


/**
 * @brief
 *     This class stores an arbitrary function pointer and runs it at any given time.
 *     As an example of use, arbitrary instances can be passed to other functions without having to declare them globally.
 *
 * Example:
 * @code
 *   using namespace mslh;
 *
 *
 *   Interrupter<Test> interrupter;
 *
 *   Class Developer {
 *   public:
 *      console() { std::cout << "hoge"; }
 *   }
 *
 *   Class Publisher {
 *   public:
 *      call() { interrupter.runSpecifiedDistance(); }
 *   }
 *
 *   int main() {
 *       Developer dev;
 *       interrupter.attach(&dev, &Developer::console);  //< ① Store any function.
 *       Publisher pub;
 *       pub.call(); //< ② Run function that stored by ①. (console() is ran.)
 *   }
 *
 * @endcode
 */
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
