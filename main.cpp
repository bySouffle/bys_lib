#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
//#include <bits/socket.h>
#include <sys/socket.h>
#include "arpa/inet.h"
#include "errno.h"
#include "string.h"
#include "sys/epoll.h"
#include "unistd.h"
#include "fcntl.h"

#include "NetBase.h"
#include "EpollServer.h"
#include <pthread.h>


void *send_data(void *){
//    int fd = socket(AF_INET,SOCK_DGRAM,0);

//    if(ret_val < 0){
//        printf("bind fail\r\n");
//    }
//    NetBase::set_socket_nonblocking(fd);

    sockaddr_in server_addr ;
    server_addr.sin_port = htons(50010);
    server_addr.sin_addr.s_addr = inet_addr("192.168.179.208");
//    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    server_addr.sin_family = AF_INET;

    char recv_buf[10] = {};
    char send_buf[100] = "123123132123132132111111111111111111111111111111111111111111111123333333333113212313";
    int send_len = 0;
    int  recv_len = 0;
    sleep(10);
    while (1){
        int fd = socket(AF_INET,SOCK_STREAM,0);
        if(fd < 0){
            Print("FD Create fail\r\n");
        }
        sockaddr_in addr;
        addr.sin_port = htons(50022);
//    addr.sin_addr.s_addr = inet_addr("192.168.179.208");
//    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_family = AF_INET;
        int ret_val = 0;

        ret_val = bind(fd, (struct sockaddr*)&addr,sizeof (struct sockaddr));
        sleep(5);
        ret_val = connect(fd,(struct sockaddr*)&server_addr,sizeof (struct sockaddr));

        Print("connect ret %d %s\r\n", ret_val, strerror(errno));
        if( ret_val >= 0){
            sleep(1);
            while (1)
            {
//                Print("send error\r\n");
                send_len = send(fd,send_buf,100,0);
                Print("send  %s\r\n", strerror(errno));

                if(send_len <= 0 ){
                    Print("send error\r\n");
                    break;
                }

                recv_len = recv(fd,recv_buf,10,0);
                if(recv_len < 0){
                    Print("recv error\r\n");
                    break;
                }
                Print("Recvbuf %s\r\n",recv_buf);
                close(fd);
                break;
            }
        }
//        sendto(fd,"123123",7,0,(struct sockaddr*)&addr,sizeof (struct sockaddr));
//        sleep(1);
    }
}

int main() {
#if 0
    int pip[2] = {};
    pipe(pip);
    NetBase base;
    base.set_socket_nonblocking(pip[0]);
//    base.set_socket_nonblocking(pip[1]);

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
//                    printf("%s\n",r);
                    printf("zzzzz!\n");
                }
            }
        }
    } else{
        for (int i = 0; i < 10; ++i) {
            char w[8] = {};

            sprintf(w,"www%d\n",i);
            if(i>2){
                sprintf(w,"zzzz%d\n",i);
            }
            write(pip[1],w,8);
//            usleep(1);
        }
        write(pip[1],"epoll!\n",8);
    }
#endif
//    printf("ZZZZZZZZZZZ\n");
//    pthread_t pid;
//    pthread_create(&pid,NULL, send_data,NULL);
//
//
//    EpollServer *epollServer = new EpollServer;
////    epollServer->epoll_udp_server_init( udp_echo_callback, 10);
//    epollServer->epoll_tcp_server_init(tcp_recv_echo_callback, 1000);
//    epollServer->epoll_server_start();
//    epollServer->epoll_server_delete();




}
