//
// Created by by Souffle on 2021/5/23.
//

#include "StringOpt.h"

long StringOpt::str16tohex(char *hex_str) {
    long hex = strtol(hex_str, nullptr, 16);
    _print("hex1: [%x]", hex);
    std::string z(hex_str);
    long h = stol(z, nullptr, 16);
    _print("hex2: [%x]", h);
    return hex;
}

//! 将 16进制数的ascii 转为 字符串 ( ex: 0x41 -> "0x41"
//! \param hex_c 16进制字符
//! \param str16 16进制显示的字符串
//! \return
void StringOpt::hextostr16(char hex_c, char *str16) {
#if 0
    unsigned char hex_low[kU4_];
    for( int i = 0; i < 2; ++i ){
        if(i%2 == 0){
            hex_low[i] = (hex_c & 0xF0) >> 4;   //  高4位
        } else{
            hex_low[i] = (hex_c & 0x0F);        //  低4位
        }
        if( hex_low[i] < 10 ){
            hex_low[i] += '0';
        } else{
            if(hex_low[i] < 16){
                hex_low[i] = hex_low[i] - 10 + 'A';
            }
        }
    }
    memcpy(str16, hex_low, kU4_);
#endif
#if 01
//    unsigned char hex_low[kU4_];
    for (int i = 0; i < 2; ++i) {
        if (i % 2 == 0) {
            str16[i] = (hex_c & 0xF0) >> 4;   //  高4位
        } else {
            str16[i] = (hex_c & 0x0F);        //  低4位
        }
        if (str16[i] < 10) {
            str16[i] += '0';
        } else {
            if (str16[i] < 16) {
                str16[i] = str16[i] - 10 + 'A';
            }
        }
    }
#endif
}

//! hex数组转为hex显示的字符串 [0x64,0x65,0x66] -> "646566"
//! \param hex_arr  hex数组指针
//! \param sstr16   返回的hex字符串指针
//! \return         操作成功返回处理的数组个数   失败返回0

int StringOpt::hextostr16_arr(char *hex_arr, char *sstr16) {
    if (hex_arr == nullptr) {
        sstr16 = nullptr;
        return 0;
    }
    int hex_arr_len = strlen(hex_arr);
    for (int i = 0; i < hex_arr_len; ++i) {

        sstr16[2 * i] = (hex_arr[i] >> 4);
        sstr16[2 * i] = hex_trans_0_F(sstr16[2 * i]);

//
//        if (sstr16[i] < 10) {
//            sstr16[i] += '0';
//        } else {
//            sstr16[i] += 'A';
//        }
        sstr16[2 * i + 1] = (hex_arr[i] & 0x0F);
        sstr16[2 * i + 1] = hex_trans_0_F(sstr16[2 * i + 1]);

//        for (int i = 0; i < hex_arr_len; ++i) {
//            sstr16[2 * i] = (hex_arr[i] >> 4);
//
//            if (sstr16[i] < 10) {
//                sstr16[i] += '0';
//            } else {
//                sstr16[i] += 'A';
//            }
//        }
    }
    sstr16[2*hex_arr_len] = 0;
    return hex_arr_len;
}


void StringOpt::int_str(int beint, char *str10) {

}

char StringOpt::hex_trans_0_F(char hex) {
    if (hex > 15) {
        return 0;
    }
    if (hex < 10) {
        hex += '0';
    } else {
        hex += 'A';
    }
    return hex;

}


