//
// Created by bys on 2021/12/7.
//

#ifndef BYS_LIB_SYSTEMMONITOR_H
#define BYS_LIB_SYSTEMMONITOR_H


#include <stdint.h>
#include <string>
#include <sys/utsname.h>

#define IN
#define OUT

typedef struct MemInfo {
    int total;
    int free;
    int buffers;
    int cached;
}MemInfo_t;


typedef struct DiskInfo {
    int total;
    int free;
    int used;
    double Usage;
}DiskInfo_t;

class SystemMonitor {
public:
    SystemMonitor();

public:
    std::string GetSysMac();

    int GetprocessCount(IN const char *process_name);

    bool IsProcessRunning(IN const char *process_name);

    int GetMemInfo(OUT MemInfo &mi);

    int GetProcessMem(pid_t pid);

    int GetTotalMem();

    double GetProcessMemPercent(pid_t pid);

    double GetTotalCpuPercent();

    double GetProcessCpuPercent(pid_t pid);

    void GetProcessCpuInfo(IN const char *process_name, OUT unsigned int *proc_nums,
                           OUT unsigned int *res);

    void GetProcessMemInfo(IN const char *process_name, OUT unsigned int *proc_nums,
                           OUT unsigned int *res);

    //check disk stat of the path ,KB,1-100%
    void GetDiskPercent(IN const char *path, OUT int *free,
                        OUT int *total, OUT int *used,  OUT double *Usage);

    void GetDiskPercent( OUT struct DiskInfo* Idisk);

    void GetSysInfo(OUT struct utsname* u);

private:
    bool IsNumeric(const char *str);

    const char *GetItems(const char *buffer, int ie);

    unsigned int GetProcessCpuOccupy(pid_t pid);

    unsigned int GetTotalCpuOccupy();

    int GetTotalCpuOccupy(void *param);
};


#endif //BYS_LIB_SYSTEMMONITOR_H
