//
// Created by ubuntu on 6/3/21.
//

#ifndef BYS_LIB_PIPEOPT_H

#include<sys/socket.h>
#include <unistd.h>

enum DefaultSize{
    kDefaultPipeSize = 65536,       //  kernel 2.6.11 default
};

enum Return_Status{
    kOK = 0,
    kError = -1,
};

class PipeOpt {
public:
    int create_pipe(int *fd);
    int create_2way_pipe(int *fd);
};


#endif //BYS_LIB_PIPEOPT_H
