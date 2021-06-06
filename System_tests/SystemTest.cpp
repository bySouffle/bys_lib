//
// Created by ubuntu on 6/2/21.
//

#include "gtest/gtest.h"
#include "SystemOpt.h"

class SystemTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        systemopt = new SystemOpt();
    }

    virtual void TearD() {
        delete systemopt;
    }

    SystemOpt *systemopt;
};

TEST_F(SystemTest, test_big_little_endian) {     // pc little endian
    int endian = systemopt->test_big_little_endian();
    EXPECT_EQ(endian, 0);

}

TEST_F(SystemTest, test_uid_euid) {
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("uid[%d]  euid[%d]\n", uid, euid);

    uid_t guid = getgid();
    uid_t geuid = getegid();
    printf("gid[%d]  egid[%d]\n", uid, euid);

}


TEST_F(SystemTest, test_setid) {     //  将uid root切换为普通usr
    int ret_val = systemopt->switch_process_to_user(503, 503);
    printf("ret_val = [%d]\n", ret_val);
}