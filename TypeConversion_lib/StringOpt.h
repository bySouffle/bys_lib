//
// Created by by Souffle on 2021/5/23.
//

#ifndef BYS_LIB_STRINGOPT_H
#define BYS_LIB_STRINGOPT_H

#define _print_flag
#ifdef _print_flag
#define _print(format, ...) printf("FILE: " __FILE__ ", LINE: %d : " format "\n" , __LINE__ , __VA_ARGS__ )
#else
#define _print(format,...)
#endif

#include <string>
#include <string.h>
#include <stdlib.h>

enum StdUnitSize {
    kU4_ = 4,
    kU8_ = 8,
    kU16_ = 16,
    kU32_ = 32,
    kU64_ = 64,
};

class StringOpt {
public:
    long str16tohex(char *hex_str);

    void hextostr16(char hex_c, char *str16);

    int hextostr16_arr(char *hex_arr, char *sstr16);

    void int_str(int be_int, char *str10);

    char hex_trans_0_F(char hex);

};


#endif //BYS_LIB_STRINGOPT_H
