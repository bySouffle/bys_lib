//
// Created by ubuntu on 6/2/21.
//

#ifndef BYS_LIB_SYSTEMOPT_H
#define BYS_LIB_SYSTEMOPT_H

enum Endianness{
    kBigEndian = 1,
    kLittleEndian = 0,
    kUnknown = -1
};


class SystemOpt {
public:
    int test_big_little_endian();

};


#endif //BYS_LIB_SYSTEMOPT_H
