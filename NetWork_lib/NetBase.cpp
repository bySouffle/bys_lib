//
// Created by ubuntu on 6/2/21.
//

#include "NetBase.h"


//! 重定向标准输出到指定fd
//! \param be_fd
//! \return
int NetBase::redirect_stdout(int be_fd) {
    close(STDOUT_FILENO);
    dup(be_fd);
    return 0;
}