//
// Created by ubuntu on 5/24/21.
//

#include "gtest/gtest.h"
#include "PipeOpt.h"
#include "NetBase.h"
#include "TcpOpt.h"
#include "UdpOpt.h"


class PipeTest :public :: testing::Test  {
protected:
    virtual void SetUp(){
        pipeopt = new PipeOpt();
    }

    virtual void TearDown(){
        delete pipeopt;
    }

    PipeOpt* pipeopt;

};

class NetBaseTest :public :: testing::Test  {
protected:
    virtual void SetUp(){
        netbase = new NetBase();
    }

    virtual void TearDown(){
        delete netbase;
    }

    NetBase* netbase;

};

class TcpOptTest :public :: testing::Test  {
protected:
    virtual void SetUp(){
        tcpopt = new TcpOpt();
    }

    virtual void TearDown(){
        delete tcpopt;
    }

    TcpOpt* tcpopt;

};

class UdpOptTest :public :: testing::Test  {
protected:
    virtual void SetUp(){
        udpopt = new UdpOpt();
    }

    virtual void TearDown(){
        delete udpopt;
    }

    UdpOpt* udpopt;

};



TEST_F(PipeTest, pipe){
    int fd[2] = {};
    int ret = pipeopt->create_pipe(fd);
    printf("ret[%d] pipe[%d] pipe[%d]\n", ret, fd[0], fd[1]);

    write(fd[1], "abcdef", 8);
    char z[8] = {};
    read(fd[0],z,sizeof (z));
    printf("read: %s\n", z);
}

TEST_F(PipeTest, way2_pipe){
    int fd[2] = {};
    int ret = pipeopt->create_2way_pipe(fd);
    int val = 0;
    if(fork()>0){
        close(fd[0]);
        while (true){
            sleep(1);
            char readbuf[16] = {};
            char writebuf[16] = {};
            sprintf(writebuf, "fwrite %d\n", val);
            write(fd[1], writebuf,sizeof (writebuf));

            read(fd[1], readbuf, sizeof(readbuf));
            printf("%s", readbuf);
            val++;
            if(val > 10){
                break;
            }
        }
    } else{
        close(fd[1]);
        while (true){

            sleep(1);
            char readbuf[16] = {};
            char writebuf[16] = {};

            sprintf(writebuf, "swrite %d\n", val);
            write(fd[0],writebuf,sizeof (writebuf));
            read(fd[0], readbuf, sizeof(readbuf));
            printf("%s", readbuf);
            val++;
            if(val > 10){
                break;
            }
        }
    }
}


TEST_F(NetBaseTest, redirect){
    if (fork() > 0) {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
//            printf("create: %s\n", strerror(errno));
        }
        sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(12345);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        if (bind(fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            printf("bind: %s\n", strerror(errno));
        }
        sockaddr_in recv_addr = {};
        socklen_t recv_addr_len;
        while (1) {
            if (listen(fd, 5) >= 0) {
                int sub_fd = accept(fd, (sockaddr *) &recv_addr, &recv_addr_len);
//                printf("port %d\n", recv_addr.sin_port);
                int i = 0;
                while (1) {
                    netbase->redirect_stdout(sub_fd);
                    printf("ZZZZZZZZ");
//                    if(send(sub_fd,"ZZZZZZZ",8,0)<0){
//                        close(sub_fd);
//                        break;
//                    }
                    sleep(1);
                    ++i;
                    if(i > 10){
                        close(sub_fd);
                        break;
                    }
                }
                if(i>10){
                    close(fd);
                    break;
                }

            }
        }

    } else{
        int m_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in ser_addr = {};
        ser_addr.sin_port = htons(12345);
        ser_addr.sin_family = AF_INET;
        inet_pton(AF_INET,"127.0.0.1",&ser_addr.sin_addr);
        socklen_t ser_len = sizeof (ser_addr);
        connect(m_fd, (sockaddr*)&ser_addr, ser_len);
        int i = 0;
        while (true){
            char readbuff[16] = {};
            if (read(m_fd,readbuff,8) <= 0 ){

                close(m_fd);
                break;
            }
            EXPECT_STREQ(readbuff,"ZZZZZZZZ");
//            printf("read: %s\n",readbuff);
            ++i;
            if (i>10){
                close(m_fd);
                break;
            }
        }
    }
}