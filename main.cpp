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

void *send_data(void *) {
//    int fd = socket(AF_INET,SOCK_DGRAM,0);

//    if(ret_val < 0){
//        printf("bind fail\r\n");
//    }
//    NetBase::set_socket_nonblocking(fd);

  sockaddr_in server_addr;
  server_addr.sin_port = htons(50010);
  server_addr.sin_addr.s_addr = inet_addr("192.168.179.208");
//    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  server_addr.sin_family = AF_INET;

  char recv_buf[10] = {};
  char send_buf[100] = "123123132123132132111111111111111111111111111111111111111111111123333333333113212313";
  int send_len = 0;
  int recv_len = 0;
  sleep(10);
  while (1) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
      Print("FD Create fail\r\n");
    }
    sockaddr_in addr;
    addr.sin_port = htons(50022);
//    addr.sin_addr.s_addr = inet_addr("192.168.179.208");
//    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    int ret_val = 0;

    ret_val = bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr));
    sleep(5);
    ret_val = connect(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

    Print("connect ret %d %s\r\n", ret_val, strerror(errno));
    if (ret_val >= 0) {
      sleep(1);
      while (1) {
//                Print("send error\r\n");
        send_len = send(fd, send_buf, 100, 0);
        Print("send  %s\r\n", strerror(errno));

        if (send_len <= 0) {
          Print("send error\r\n");
          break;
        }

        recv_len = recv(fd, recv_buf, 10, 0);
        if (recv_len < 0) {
          Print("recv error\r\n");
          break;
        }
        Print("Recvbuf %s\r\n", recv_buf);
        close(fd);
        break;
      }
    }
//        sendto(fd,"123123",7,0,(struct sockaddr*)&addr,sizeof (struct sockaddr));
//        sleep(1);
  }
}

#include <arpa/inet.h>      // inet_ntop()
#include <netinet/in.h>
//--//
#include "check_ip.h"
//--//
#include <iostream>
#include <string>
int main(int argc, char*argv[]) {
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
#if 0
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
/* Include necessary libraries */

  {
    addrinfo *result;
    sockaddr_in *pDestinationAddr;
    char ipString[INET_ADDRSTRLEN];

    if (argc < 2){
      return -1;
    }
    // get IP Address and store in 'result' (passed by reference)
    if (getIPAddress(argv[1], result) != 0) {
      std::cout << "Invalid IP Address. Terminating ...\n";
      exit(EXIT_FAILURE);
    } else {
      pDestinationAddr =
          (sockaddr_in *) result->ai_addr;                           // get struct from resulting linked list
      void *address;
      address = &pDestinationAddr->sin_addr;                                      // store IP Address
      inet_ntop(result->ai_family, address, ipString, sizeof(ipString));          // convert binary IP to string
      std::cout << "IP: " << ipString << std::endl;
    }

    // get appropriate socket file descriptor
    int socketFD = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socketFD < 0) {
      std::cout << socketFD << "Socket File Descriptor creation failed .\n";
      close(socketFD);
      exit(EXIT_FAILURE);
    } else {
      std::cout << "Socket File Descriptor: " << socketFD << std::endl;
    }

    // set socket option to TTL (time to live)
    if (setsockopt(socketFD, IPPROTO_IP, IP_TTL, &TimeToLive, sizeof(TimeToLive)) != 0) {
      std::cout << "Setting Socket Options failed.\n";
      exit(EXIT_FAILURE);
    } else {  // for debugging purposes
      std::cout << "Socket Option Set to TTL.\n";
    }

    // declare and initialize struct
    timeval TimeOut = {ReceivalTimeOut, 0};

    // set timeout of receival
    if (setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (const char *) &TimeOut, sizeof(TimeOut)) != 0) {
      std::cout << "Setting Timeout for Receival failed.\n";
      exit(EXIT_FAILURE);
    } else {  // for debugging purposes
      std::cout << "Timeout for receival set.\n";
    }

    // free linked list before infinite loop
    freeaddrinfo(result);

    // variables to track packets
    int transmitted = 0;

    std::cout << "\n-----\n\n";

    int success = 0, failure = 0;

    // ping in infinite loop
    for (;;) {
      int flag = sendPing(socketFD, transmitted, pDestinationAddr, success, failure);
      std::cout << flag << "\n";
      if (flag != -1) {
        // add delay of 0.5 sec
        usleep(500000);
      }
    }

    // never reached
    close(socketFD);

  }
#endif
  PingFunc func;
  std::string ip("127.0.0.1");
  std::cout << ip << "\n";
  func.set_ip(ip);
  int ret_val = func.config();
  if( ret_val < 0){
    std::cout << ret_val << " ping error\n";
    return -1;
  }

  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  func.start_ping();
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";


  sleep(2);

  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";

  std::cout << "is ping? " << func.is_ping() << "\n";
  func.channel_ping();
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";

  std::cout << "is ping? " << func.is_ping() << "\n\n";


  ip = "192.168.100.186";
  func.set_ip(ip);
  func.config();
  if( ret_val < 0){
    std::cout << ret_val << " ping error\n";
    return -1;
  }

  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";

  func.start_ping();
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";

  sleep(2);
  std::cout << "is ping? " << func.is_ping() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";

  func.channel_ping();
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";
  std::cout << "=========== " << func.GetIsError() << "\n";





}
