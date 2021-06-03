//
// Created by ubuntu on 6/3/21.
//

#ifndef BYS_LIB_PIPEOPT_H
#define BYS_LIB_PIPEOPT_H

#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>

enum DefaultSize{
    kDefaultPipeSize = 65536,       //  kernel 2.6.11 default
};

class PipeOpt {

};


#endif //BYS_LIB_PIPEOPT_H
