//
// Created by ubuntu on 6/2/21.
//

#include "SystemOpt.h"
//! 测试系统的字节序
//! \return 大端:1    小端:0    未知:-1
int SystemOpt::test_big_little_endian() {
    union {
        short value = 0x0102;
        char byte[sizeof (value)];
    }double_bytes;

    if( (double_bytes.byte[0] == 1) && (double_bytes.byte[1] == 2) ){
        return kBigEndian;
    } else if( (double_bytes.byte[0] == 2) && (double_bytes.byte[1] == 1) ) {
        return kLittleEndian;
    } else{
        return kUnknown;
    }

    return 0;
}
