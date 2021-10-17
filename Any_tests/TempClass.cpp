//
// Created by bysou on 2021/9/21.
//

#include "TempClass.h"

void THREAD_TEST::func1(int a) {
    printf("thread_prt : %d\r\n", a);
}

THREAD_TEST::THREAD_TEST() {

}

void THREAD_TEST::func2(int a) {
    m_a1 = a;
}

void THREAD_TEST::print_m_a1() {
    printf("m_a1 = %d\r\n", m_a1);

}

int TEST_2::func_1() {
    return 0;
}
