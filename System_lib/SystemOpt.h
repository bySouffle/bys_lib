//
// Created by ubuntu on 6/2/21.
//

#ifndef BYS_LIB_SYSTEMOPT_H
#define BYS_LIB_SYSTEMOPT_H



/* ===================== 日志系统 ==============================
 * 1. 用户进程日志
 *  #include<syslog.h>
 *  void syslog(int priority,const char*message,...);
 *  param：
 *      priority 日志级别：（按位或）
 *      #define LOG_EMERG 0     //  系统不可用
 *      #define LOG_ALERT 1     //  报警，需要立即采取动作
 *      #define LOG_CRIT 2      //  非常严重的情况
 *      #define LOG_ERR 3       //  错误
 *      #define LOG_WARNING 4   //  警告
 *      #define LOG_NOTICE 5    //  通知
 *      #define LOG_INFO 6      //  信息
 *      #define LOG_DEBUG 7     //  调试

 *
 *  功能：改变默认输出方式
 *  #include<syslog.h>
 *  void openlog(const char*ident,int logopt,int facility);
 *  param:
 *      ident: 参数指定的字符串将被添加到日志消息的日期和时间之后，它通常被设置为程序的名字
 *      logopt: 按位或
 *              #define LOG_PID 0x01    //  在日志消息中包含程序PID
 *              #define LOG_CONS 0x02   //  如果消息不能记录到日志文件，则打印至终端
 *              #define LOG_ODELAY 0x04 //  延迟打开日志功能直到第一次调用syslog
 *              #define LOG_NDELAY 0x08 //  不延迟打开日志功能
 *      facility: 修改syslog函数中的默认设施值
 *
 *
 *  功能：日志掩码，日志级别大于掩码的日志信息被忽略
 *  #include<syslog.h>
 *  int setlogmask(int maskpri);
 *
 *
 *  功能：关闭日志
 *  void closelog();
 *
 */

/*  ====================    用户信息    ============================
 *  EUID 方便资源访问
 *
 *  1. uid & euid
 *  #include<sys/types.h>
 *  #include<unistd.h>
 *  uid_t getuid();         //  获取真实用户ID
 *  uid_t geteuid();        //  获取有效用户ID
 *  gid_t getgid();         //  获取真实组ID
 *  gid_t getegid();        //  获取有效组ID
 *
 *  2. gid & egid
 *  int setuid(uid_t uid);  //  设置真实用户ID
 *  int seteuid(uid_t uid); //  设置有效用户ID
 *  int setgid(gid_t gid);  //  设置真实组ID
 *  int setegid(gid_t gid); //  设置有效组ID
 *
 *  3. pid & gpid
 *  pid_t getpgid(pid_t pid);
 *  int setpgid(pid_t pid,pid_t pgid);
 *
 *
 *  会话
 *  pid_t setsid(void);     //  不能由进程组首领进程调用
 *      (1) 调用进程成为会话首领
 *      (2) 新建一个进程组，其PGID就是调用进程的PID，调用进程成为该组的首领
 *      (3) 调用进程将甩开终端
 *
 *  会话{ 进程组 进程组 }   linux进程无SID概念 系统认为它等于会话首领所在的进程组的PGID
 *  pid_t getsid(pid_t pid);    //  获取进程组
 *
 *  { "sid 1943":{ "pgid 1943": bash, "pgid 2298":{ ps, less }  } }
 *
 *
 */


/*  =================== 系统资源限制  ===========================
 *  #include＜sys/resource.h＞
 *  int getrlimit(int resource,struct rlimit*rlim);
 *  int setrlimit(int resource,const struct rlimit*rlim);
 *
 *      1. resource
 *          RLIMIT_AS           进程虚拟内存总量限制
 *          RLIMIT_CORE         进程核心转储文件大小(bytes)
 *          RLIMIT_CPU          进程CPU时间限制
 *          RLIMIT_DATA         进程数据段(初始化数据data段, 未初始化数据bss段,堆)限制
 *          RLIMIT_FSIZE        文件大小限制
 *          RLIMIT_NOFILE       fd限制
 *          RLIMIT_NPROC        用户能创建的进程数量
 *          RLIMIT_SIGPENDING   能够挂起的信号数量
 *          RLIMIT_STACK        进程栈内存限制
 *
 *
 *  struct rlimit{
 *      rlim_t rlim_cur;    //  软限制, 建议.超出报错
 *      rlim_t rlim_max;    //  硬限制
 *  }
 *
 *  cmd: ulimit
 *
 */

/*  =================   守护进程    ===================
 *  #include＜unistd.h＞
 *  int daemon(int nochdir,int noclose);
 *  param:
 *      nochdir     0==>"/"
 *      noclose     0==>std in out error ==> "/dev/null"
 *
 *  return:
 *      success:0   fail:-1
 *
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


enum RetBaseStatus {
    kOK = 0,

};

enum Endianness {
    kBigEndian = 1,
    kLittleEndian = 0,
    kUnknown = -1
};

enum RetSwitchUsr {
    kUsrIdError = -1,
    kIsUsrError = -2,
    kIdSetError = -3,
};

class SystemOpt {
public:
    int test_big_little_endian();
    int switch_process_to_user(uid_t usrid, gid_t gp_id);
    bool daemonize();

};


#endif //BYS_LIB_SYSTEMOPT_H
