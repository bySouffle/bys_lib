//
// Created by bys on 2021/12/8.
//

#ifndef BYS_LIB_THREADOUTSYSINFO_H
#define BYS_LIB_THREADOUTSYSINFO_H
#include "SystemMonitor.h"
#include <thread>
#include <string.h>

class ThreadOutSysInfo {
public:
    ThreadOutSysInfo();
   ~ThreadOutSysInfo();

    void start();
    void stop();
    void routine_task(void* param);
private:

    int exit_flag_;
    std::string msg;
    std::string mac_;
    double cpu_usage_;
    struct utsname *u;

    DiskInfo_t *disk_info_;
    MemInfo_t *mem_info_;

    SystemMonitor *s_;
    std::thread *t_{};

};


#endif //BYS_LIB_THREADOUTSYSINFO_H
