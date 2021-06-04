//
// Created by ubuntu on 6/3/21.
//

#include "PipeOpt.h"
//! 创建一个双向管道
//! 进程中通信 父子进程间分别关闭一个fd
//! \param fd   int &fd[2]
//! \return     success:0   fail:-1

int PipeOpt::create_2way_pipe(int *fd) {
    int ret_val = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    if (ret_val < 0){
        return kError;
    }
    return kOK;
}

//! 创建一个管道
//! \param fd   int &fd[2]
//! \return     success:0   fail:-1
int PipeOpt::create_pipe(int *fd) {
    int ret_val = pipe(fd);
    if(ret_val<0){
        return kError;
    }
    return kOK;
}



