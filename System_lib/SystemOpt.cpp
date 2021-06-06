//
// Created by ubuntu on 6/2/21.
//

#include "SystemOpt.h"

//! 测试系统的字节序
//! \return 大端:1    小端:0    未知:-1
int SystemOpt::test_big_little_endian() {
    union {
        short value = 0x0102;
        char byte[sizeof(value)];
    } double_bytes;

    if ((double_bytes.byte[0] == 1) && (double_bytes.byte[1] == 2)) {
        return kBigEndian;
    } else if ((double_bytes.byte[0] == 2) && (double_bytes.byte[1] == 1)) {
        return kLittleEndian;
    } else {
        return kUnknown;
    }

    return 0;
}

int SystemOpt::switch_process_to_user(uid_t usr_id, gid_t gp_id) {
    //  确保切换的用户不为root
    if ((usr_id == 0) && (gp_id == 0)) {
        return kUsrIdError;
    }
    //  确保当前用户合法
    uid_t uid = getuid();
    gid_t gid = getgid();
    if (((gid != 0) || (uid != 0)) && ((gid != gp_id) || (uid != usr_id))) {
        return kIsUsrError;
    }
    if (uid != 0) {   //  已经是usr

        return kOK;
    }
    if ((setuid(usr_id) < 0) || (setgid(gp_id) < 0)) {//  切换到指定用户id
        return kIdSetError;
    }

    return kOK;
}
