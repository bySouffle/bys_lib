//
// Created by ubuntu on 5/24/21.
//

#ifndef BYS_LIB_FILEOPT_H
#define BYS_LIB_FILEOPT_H

#define __printf_
#ifdef __printf_
#define _print(format,...) printf("FILE: " __FILE__ ", LINE: %d " format "\n" , __LINE__ , __VA_ARGS__)
#else
#define _print(format,...)
#endif

#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"
#include <string.h>
#include <string>
#include "fcntl.h"
#include <syslog.h>


enum LimitSize{
    kMaxFNameSize = 255,
    kStdFNameSize = 128,
    kMaxUrlSize = 4096,
    kStdUrlSize = 256,

};

enum ReturnStatus{
    kInvalid = -1,
    kOk = 0,
    kMapObjNumberError = -2,
    kOpenFileError = -3,
    kGetIndexError = -4,
};


class FileOpt {
public:
    std::string get_current_url() ;
    int judge_dir_exist_and_create(const std::string &file_url);
    int judge_dir_exist_and_create(const char *file_url);

    int set_fd_nonblocking(int fd);

};


#endif //BYS_LIB_FILEOPT_H
