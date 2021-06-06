//
// Created by ubuntu on 5/24/21.
//
#define _GNU_SOURCE

#include "fcntl.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/uio.h"
#include "gtest/gtest.h"
#include "PipeOpt.h"
#include "NetBase.h"
#include "TcpOpt.h"
#include "UdpOpt.h"


class PipeTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        pipeopt = new PipeOpt();
    }

    virtual void TearDown() {
        delete pipeopt;
    }

    PipeOpt *pipeopt;

};

class NetBaseTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        netbase = new NetBase();
    }

    virtual void TearDown() {
        delete netbase;
    }

    NetBase *netbase;

};

class TcpOptTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        tcpopt = new TcpOpt();
    }

    virtual void TearDown() {
        delete tcpopt;
    }

    TcpOpt *tcpopt;

};

class UdpOptTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        udpopt = new UdpOpt();
    }

    virtual void TearDown() {
        delete udpopt;
    }

    UdpOpt *udpopt;

};


TEST_F(PipeTest, pipe) {
    int fd[2] = {};
    int ret = pipeopt->create_pipe(fd);
    printf("ret[%d] pipe[%d] pipe[%d]\n", ret, fd[0], fd[1]);

    write(fd[1], "abcdef", 8);
    char z[8] = {};
    read(fd[0], z, sizeof(z));
    printf("read: %s\n", z);
}

TEST_F(PipeTest, way2_pipe) {
    int fd[2] = {};
    int ret = pipeopt->create_2way_pipe(fd);
    int val = 0;
    if (fork() > 0) {
        close(fd[0]);
        while (true) {
            sleep(1);
            char readbuf[16] = {};
            char writebuf[16] = {};
            sprintf(writebuf, "fwrite %d\n", val);
            write(fd[1], writebuf, sizeof(writebuf));

            read(fd[1], readbuf, sizeof(readbuf));
            printf("%s", readbuf);
            val++;
            if (val > 10) {
                break;
            }
        }
    } else {
        close(fd[1]);
        while (true) {

            sleep(1);
            char readbuf[16] = {};
            char writebuf[16] = {};

            sprintf(writebuf, "swrite %d\n", val);
            write(fd[0], writebuf, sizeof(writebuf));
            read(fd[0], readbuf, sizeof(readbuf));
            printf("%s", readbuf);
            val++;
            if (val > 10) {
                break;
            }
        }
    }
}


TEST_F(NetBaseTest, redirect) {
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
                    if (i > 10) {
                        close(sub_fd);
                        break;
                    }
                }
                if (i > 10) {
                    close(fd);
                    break;
                }

            }
        }

    } else {
        int m_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in ser_addr = {};
        ser_addr.sin_port = htons(12345);
        ser_addr.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &ser_addr.sin_addr);
        socklen_t ser_len = sizeof(ser_addr);
        connect(m_fd, (sockaddr *) &ser_addr, ser_len);
        int i = 0;
        while (true) {
            char readbuff[16] = {};
            if (read(m_fd, readbuff, 8) <= 0) {

                close(m_fd);
                break;
            }
            EXPECT_STREQ(readbuff, "ZZZZZZZZ");
//            printf("read: %s\n",readbuff);
            ++i;
            if (i > 10) {
                close(m_fd);
                break;
            }
        }
    }
}

TEST_F(NetBaseTest, readv_writev) {
    if (fork() > 0) {


        int ser_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in ser_addr = {};
        ser_addr.sin_family = AF_INET;
        ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        ser_addr.sin_port = htons(23333);
        if (0 != bind(ser_fd, (sockaddr *) &ser_addr, sizeof(ser_addr))) {
            printf("errno[%s]\n", strerror(errno));
        }
        while (true) {
            //  成功
            if (listen(ser_fd, 5) == 0) {
                sockaddr_in conn_addr = {};
                socklen_t con_addrlen = 0;

                int conn_fd = accept(ser_fd, (sockaddr *) &conn_addr, &con_addrlen);
                if (conn_fd > 0) {
                    char head_buf[1024] = {};
                    char *file_buf = nullptr;
                    struct stat file_stat = {};
                    int file_valid = true;
                    int len = 0;
                    if (stat("Makefile", &file_stat) < 0) {
                        file_valid = false;
                    } else {
                        if (S_ISDIR(file_stat.st_mode)) {
                            file_valid = false;
                        } else if (file_stat.st_mode & S_IROTH) {
                            int file_fd = open("Makefile", 0775);
                            file_buf = new char[file_stat.st_size + 1];
                            memset(file_buf, 0, sizeof(file_buf));
                            if (read(file_fd, file_buf, file_stat.st_size) < 0) {
                                file_valid = false;
                            }
                        } else {
                            file_valid = false;
                        }
                    }

                    if (file_valid == true) {
                        int ret_len = snprintf(head_buf, 1024 - 1,
                                               "%s%s\r\n", "HTTP/1.1", "200 OK");
                        len += ret_len;
                        ret_len = snprintf(head_buf + len, 1024 - 1 - len, "Content-Length:%d\r\n", file_stat.st_size);
                        len += ret_len;
                        ret_len = snprintf(head_buf + len, 1024 - 1 - len, "%s", "\r\n");

                        struct iovec iv[2] = {};
                        iv[0].iov_base = head_buf;
                        iv[0].iov_len = strlen(head_buf);

                        iv[1].iov_base = file_buf;
                        iv[1].iov_len = strlen(file_buf);
                        int ret = writev(conn_fd, iv, 2);
                    } else {
                        char buf_1[8] = "aaaaaa\n";
                        char buf_2[8] = "zzzzzz\n";
                        struct iovec iv[2] = {};
                        iv[0].iov_base = buf_1;
                        iv[0].iov_len = strlen(buf_1);
                        iv[1].iov_base = buf_2;
                        iv[1].iov_len = strlen(buf_2);

                        writev(conn_fd, iv, 2);
//                        send(conn_fd, "zzzzzz\n", 8, 0);
                    }
                    if (file_buf != nullptr) {
                        delete[]file_buf;
                    }
                    close(conn_fd);
                    break;
                }

            }

        }
        close(ser_fd);
    } else {

        sleep(1);
        int cli_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in sser_addr;
        sser_addr.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &sser_addr.sin_addr);
        sser_addr.sin_port = htons(23333);
        if (0 == connect(cli_fd, (sockaddr *) &sser_addr, sizeof(sser_addr))) {
            printf("connect succeess!\n");
            char readbuf[1024] = {};
//            int ret_rlen = recv(cli_fd,readbuf, 1024-1,0);
            iovec recv_iv[2] = {};
            char iv_1buf[8] = {};
            recv_iv[0].iov_base = iv_1buf;
            recv_iv[0].iov_len = 8;

            char iv_2buf[8] = {};
            recv_iv[1].iov_base = iv_2buf;
            recv_iv[1].iov_len = 8;
            readv(cli_fd, recv_iv, 2);
//            memcpy(readbuf,recv_iv[0].iov_base,recv_iv[0].iov_len);
            printf("%s\n", iv_1buf);
//            printf("%s\n",readbuf);
        }
        close(cli_fd);

    }

}


TEST_F(NetBaseTest, Retroreflectserver) {       // mac test fail, have not splice
    if (fork() > 0) {
        int ser_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in ser_addr = {};
        ser_addr.sin_family = AF_INET;
        ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        ser_addr.sin_port = htons(23333);

        if (bind(ser_fd, (sockaddr *) &ser_addr, sizeof(sockaddr)) != -1) {
            sockaddr_in conn_addr = {};
            socklen_t conn_len = {};

            if (listen(ser_fd, 5) != -1) {
                int conn_fd = accept(ser_fd, (sockaddr *) &conn_addr, &conn_len);
                if (conn_fd > 0) {
                    int pipefd[2] = {};
                    int ret_pipval = pipe(pipefd);
                    int ret = splice(conn_fd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
                    ret = splice(pipefd[0], NULL, conn_fd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
                    close(conn_fd);
                }
            }
        }
        close(ser_fd);

    } else{
        sleep(1);
        int cli_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in con_addr = {};
        int ret_val =  connect(cli_fd, (sockaddr*)&con_addr,sizeof (con_addr));
        if(ret_val >=0){
            send(cli_fd,"zzzzzzz",8,0);
            while (true){
                char recv_buf [8] = {};
                long recv_len = recv(cli_fd,recv_buf,8,0);
                if(recv_len>0){
                    printf("recvdata:[%s]\n", recv_buf);
                    close(cli_fd);
                    break;
                }
            }
        }
    }
}
