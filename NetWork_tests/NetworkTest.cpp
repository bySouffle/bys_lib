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
#include "dsp_socket.h"
#include "sys/epoll.h"
#include "stdio.h"


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

//  DSP socket test
class DspSocketTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        dspsocket = new DspSocket();
    }

    virtual void TearDown() {
        delete dspsocket;
    }

    DspSocket *dspsocket;
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

/*
 *  1. ET模式下在缓存区不取出
 *  多次写未触发
 *
 *  zzzzz!
 *
 *
 *
 */
TEST(epoll, ET){
    int pip[2] = {};
    pipe(pip);
    NetBase base;
    base.set_socket_nonblocking(pip[0]);
    if (fork()>0){
        //  1.  epoll_FD wait_FD
        int epfd = 0;
        int nfds = 0;
        //  2.  struct epoll event
        struct epoll_event ev = {};         //  register event struct
        struct epoll_event events[5] = {};  //  deal with event struct
        //  3.  create listening fd
        epfd = epoll_create(1);
        //  4.  set listening param
        ev.data.fd = pip[0];            //  listening fd
        ev.events = EPOLLIN | EPOLLET;  //  listening mode  ET/LT
//        ev.events = EPOLLIN ;  //  listening mode  ET/LT

        /*
         * 5.  register epoll event
         *  (1) epfd
         *  (2) ctl_option
         *  (3) be listening fd
         *  (4) struct event
         */
        epoll_ctl(epfd, EPOLL_CTL_ADD, pip[0], &ev);
        for(;;){
            //  6.  cyclic
            //  (1) epfd
            //  (2) events
            //  (3) max number
            //  (4) timeout -1
            nfds = epoll_wait(epfd, events, 5, -1);
            for (int i = 0; i < nfds; ++i) {
                if (events[i].data.fd == pip[0] ){
                    char r[8] = {};
//                    read(pip[0], r,8);
                    printf("zzzzz!\n");
                }
            }
        }
    } else{
        for (int i = 0; i < 10; ++i) {
            char w[8] = {};
            sprintf(w,"%d",i);
            write(pip[1],w,8);
        }
//        write(pip[1],"epoll!\n",8);
    }
}




void ip_calc_ipchecksum_2(void *pack_info){
    int8_t* m_pack_info = (int8_t*)pack_info  ;
    m_pack_info[24] = 0;
    m_pack_info[25] = 0;

    m_pack_info = &m_pack_info[13];
    printf("m_pack_info: %x\r\n", *m_pack_info++);
    int index = 0;
    int ip_index = 0;

    int32_t calc_sum = 0;
    int16_t ip_head[10] = {};
    int32_t cksum = 0;

    while (index != 20)
    {
        /* code */
        ip_head[ip_index] = ( ip_head[ip_index] |( m_pack_info[index] & 0xFF) ) << 8;
        printf("m_pack_info[%d] = %x\r\n", index,  m_pack_info[index] );

        ip_head[ip_index] = ( ip_head[ip_index] | (m_pack_info[index+1] & 0xFF) );
        printf("m_pack_info[%d] = %x\r\n", index+1,  m_pack_info[index+1] );

        printf("ip_head[%d] = %x\r\n",ip_index,  ip_head[ip_index] );

        calc_sum += (ip_head[ip_index] & 0xffff );


        printf("zzzzzzzzzzz%d %x %x\r\n", index, ip_head[ip_index], calc_sum);
        index += 2;
        ip_index += 1;
    }
    if(index == 20){
        calc_sum=(calc_sum>>16)+(calc_sum&0xffff);  //把高位的进位，加到低八位，其实是32位加法
        calc_sum+=(calc_sum>>16);  //add carry
        // calc_sum = 0x1e21;
        calc_sum = 0xd967;
        cksum = ( ((0xF-(calc_sum)&0xF000) >> 12) << 12 |
                  ((0xF-(calc_sum)&0x0F00) >> 8 ) << 8 ) ;

        cksum = cksum |  ((0xF-(calc_sum)&0x00F0) >> 4 ) << 4  |
                (0xF-(calc_sum)&0x000F) ;
        /*
        cksum = ( ((0xF-(calc_sum)&0xF000) >> 12) << 12 |
                  ((0xF-(calc_sum)&0x0F00) >> 8 ) << 8  |
                  ((0xF-(calc_sum)&0x000F) >> 4 ) << 4  |
                  (0xF-(calc_sum)&0x000F) );
        */
        printf("%x %x %x %x\r\n",   ((0x000F-((calc_sum)&0xF000) >> 12) ) ,
               ((0x000F-((calc_sum)&0x0F00) >> 8) ),
               ((0x000F-((calc_sum)&0x00F0) >> 4) ),
               (0x000F-(calc_sum)&0x000F));
        // cksum=~calc_sum;   //取反\

        printf("1111111ck_sum = %x\r\n", calc_sum);
        printf("2222222ck_sum = %x\r\n", cksum);
    }


}

TEST(ip_calc_ipchecksum_2, CHECK){
    uint8_t iphead[] = {
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
            0xff, 0xff,

            //  Type
            0x45, 0x00,
            //  Length
            0x00, 0x25,
            //  Identification
            0xd4, 0xc4,
            //  flag offset
            0x00, 0x00,
            //  TTL Protocol
            0x80, 0x11,
            //  CheckSum    !!!!!
            0x11, 0x11,
            //  src ip
            0xc0, 0xa8, 0x01, 0x6f,
            //  dst ip
            0xc0, 0xa8, 0x01, 0x65,

            //  padding
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

    };

    ip_calc_ipchecksum_2(iphead);

}




TEST(DspSocket, ip_checksum){
    DspSocket *dspsocket = new DspSocket;
    uint8_t srcmac[6] = {0x2c,0xf0,0x5d,0x16,0xff,0x57};
    uint8_t dstmac[6] = {0x2c,0x16,0xdb,0xa1,0x94,0xf1};

    dspsocket->init_mac(&dspsocket->mac_info_, srcmac,dstmac);
    dspsocket->init_mac_type(&dspsocket->mac_info_, 0x08);

    dspsocket->init_ip_headinfo(&dspsocket->ip_info_, 0x45);
    dspsocket->init_ip_dsf(&dspsocket->ip_info_, 0x00);
    //  TODO TOTAL LENGTH
    dspsocket->set_ip_length(&dspsocket->ip_info_, 0x25);

    dspsocket->only_ide(&dspsocket->ip_info_, 0xd4c5);
    dspsocket->init_flag_offset(&dspsocket->ip_info_, 0x00);
    dspsocket->init_ttl(&dspsocket->ip_info_, 0x80);
    dspsocket->init_protocol(&dspsocket->ip_info_, 0x11);
    uint8_t srcip[4] = {0xc0,0xa8,0x01,0x6f};
    uint8_t dspip[4] = {0xc0,0xa8,0x01,0x65};
    dspsocket->init_ip_addr(&dspsocket->ip_info_, srcip, dspip);
    dspsocket->init_ip_addr(&dspsocket->ip_info_, srcip, dspip);

    dspsocket->init_src_port(&dspsocket->udp_info_, 0xd431);
    dspsocket->init_dst_port(&dspsocket->udp_info_, 0x3039);
    dspsocket->set_udp_length(&dspsocket->udp_info_, 0x11);
    memcpy(&dspsocket->udp_packet_.mac_info,&dspsocket->mac_info_,sizeof (dspsocket->mac_info_));
    memcpy(&dspsocket->udp_packet_.ip_info,&dspsocket->ip_info_,sizeof (dspsocket->ip_info_));
    memcpy(&dspsocket->udp_packet_.udp_info,&dspsocket->udp_info_,sizeof (dspsocket->udp_info_));

    uint8_t m_data[9] = {0x31,0x32,0x33,0x31,0x32,0x33,0x31,0x32,0x33};
    dspsocket->dsp_adddata(&dspsocket->udp_packet_, m_data, 9);

    dspsocket->set_ip_checksum(&dspsocket->udp_packet_);
    dspsocket->set_udp_checksum(&dspsocket->udp_packet_);

}