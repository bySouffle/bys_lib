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

    virtual void TearDown() {
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

TEST_F(SystemTest, test_chroot_chdir){
    char current_dir[1024] = {};
    char new_dir[1024] = {};

    getcwd(current_dir,1024);
    printf("current:[%s]\n",current_dir);

    strcat(current_dir,"/test");
    memcpy(new_dir,current_dir,1024);

    mkdir(current_dir,0775);
    chdir(current_dir);

    getcwd(current_dir,1024);
    printf("change current:[%s]\n",current_dir);

    chdir(new_dir);
    chroot(new_dir);

    chdir("./test");
    getcwd(current_dir, 1024);
    printf(" / change current:[%s]\n",current_dir);


    chdir("./");
    getcwd(current_dir, 1024);
    printf(" / change current:[%s]\n",current_dir);

}

TEST_F(SystemTest, test_daemonize) {    //  test daemon
    int ret_val = systemopt->daemonize();
    EXPECT_EQ(ret_val, 1);
}