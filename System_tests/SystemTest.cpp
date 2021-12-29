//
// Created by ubuntu on 6/2/21.
//

#include "gtest/gtest.h"
#include "SystemOpt.h"
#include "SystemMonitor.h"
#include "ThreadOutSysInfo.h"

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

TEST(sys_monitor, test_sys_info){
    SystemMonitor inst;
    double cpu_usage = 0;
    cpu_usage = inst.GetTotalCpuPercent();
    std::cout << "cpu_usage: " << cpu_usage << std::endl;
    MemInfo mem = {};
//    int used, free, total;
    inst.GetMemInfo(mem);
    std::cout << "buffers: " << mem.buffers/1024/1024 << " free: " << mem.free/1024/1024 << " total: " << mem.total/1024/1024 << std::endl;

    const char* path ="/";
    int disk_free;
    int disk_total;
    int disk_used;

    double  usage;
    inst.GetDiskPercent(path, &disk_free, &disk_total, &disk_used, &usage);
    printf("disk : disk_free %d, disk_total %d, disk_used %d, usage %.2f ",
           disk_free, disk_total, disk_used, usage);

    std::string mac;
    mac.append(inst.GetSysMac()) ;
    std::cout << "mac: " << mac << std::endl;

    int count = inst.GetprocessCount("top");
    std::cout << "top: " << count << std::endl;

}

TEST(sys_info, version) {
    SystemMonitor s;
    struct utsname u = {};
    s.GetSysInfo(&u);
}

TEST(thread_sys_info, test) {
    ThreadOutSysInfo t;
    t.start();
    sleep(10);
    t.stop();

}