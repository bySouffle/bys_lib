//
// Created by bysou on 2021/9/21.
//

#ifndef BYS_LIB_TEMPCLASS_H
#define BYS_LIB_TEMPCLASS_H

#include <stdio.h>
#include <stdlib.h>
class TempClass {

};


class THREAD_TEST{

public:
    THREAD_TEST();
    void func1(int a);
    void func2(int a);
    void print_m_a1();
private:
    int m_a1;

};

class TEST_2{
public:
    int func_1();
private:

};
#endif //BYS_LIB_TEMPCLASS_H
