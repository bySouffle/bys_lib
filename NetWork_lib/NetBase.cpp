//
// Created by ubuntu on 6/2/21.
//

#include "NetBase.h"


//! 重定向标准输出到指定fd
//! \param be_fd    被重定向的文件描述符
//! \return         success:0 fail:-1
int NetBase::redirect_stdout(int be_fd) {
    close(STDOUT_FILENO);
    int ret_val = dup(be_fd);
    return ret_val;
}

//! 关闭旧的fd并重定向
//! \param old_fd
//! \param new_fd
//! \return
int NetBase::redirect_fd(int old_fd, int new_fd) {
    int ret_val = dup2(old_fd, new_fd);
    return ret_val;
}

//! 设置fd为非阻塞状态
//! \param fd
//! \return [-1: 获取fd状态失败] [-2: 设置fd状态失败]
int NetBase::set_socket_nonblocking(int fd) {
    int old_fd_option = fcntl(fd, F_GETFL);
    if(old_fd_option < 0){
        return -1;
    }
    if( fcntl(fd, F_SETFL, old_fd_option | O_NONBLOCK) < 0 ){
        return -2;
    }

    return old_fd_option;
}

