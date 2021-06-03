//
// Created by ubuntu on 5/24/21.
//

#include "gtest/gtest.h"
#include "PipeOpt.h"

class NetworkTest :public :: testing::Test  {
protected:
    virtual void SetUp(){
        pipeopt = new PipeOpt();
    }

    virtual void TearDown(){
        delete pipeopt;
    }

    PipeOpt* pipeopt;
};


TEST_F(NetworkTest, pipe){
    int fd[2] = {};
    int ret = pipe(fd);
    printf("ret[%d] pipe[%d] pipe[%d]\n", ret, fd[0], fd[1]);

    write(fd[1], "abcdef", 8);
    char z[8] = {};
    read(fd[0],z,sizeof (z));
    printf("read: %s\n", z);
}