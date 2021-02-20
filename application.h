/**
 * 2021/02/13
 * @author KuraZuzu
 * */

#ifndef ZUZUHALFTPPMOD1_APPLICATION_H
#define ZUZUHALFTPPMOD1_APPLICATION_H



#ifdef __cplusplus

#include "machine.h"


extern "C" {

Machine test;

void test_move();
void test_buzzer();
void test_led();

#endif

#ifdef __cplusplus

}
#endif


#endif // ZUZUHALFTPPMOD1_APPLICATION_H