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
 *  3. 切换用户
 *
 *
 *
 *
 */

#include <unistd.h>

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
};


#endif //BYS_LIB_SYSTEMOPT_H
