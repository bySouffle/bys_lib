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

int main() {
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
}
