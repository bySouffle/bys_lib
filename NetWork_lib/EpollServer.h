//
// Created by souffle on 8/31/21.
//

#ifndef BYS_LIB_EPOLLSERVER_H
#define BYS_LIB_EPOLLSERVER_H

//  create socket
#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <vector>
//  close socket
#include<unistd.h>

#include <errno.h>

#include "NetBase.h"
#include "ScalableBuff.h"
#define __DEBUG__

#ifdef __DEBUG__
#define print(format,...) printf("file: " __FILE__ "line: %d " format "\n", __LINE__ , ##__VA_ARGS__ )
#elif
#define print(format,...)
#endif

typedef enum EPOLL_SERVER_TYPE{
    UDP_PROTO = 0,
    TCP_PROTO = 1,
}epoll_server_type_t;

enum LIMIT_PARAM{
    kMaxEpollSize = 10000,
    kMaxLine      = 10240,
    kMaxListen    = 100,
};

enum ERROR_MAPPING{
    ERROR_BASE = -128,
    CREATE_TCP_SOCKET_ERROR = ERROR_BASE - 10,
    CREATE_UDP_SOCKET_ERROR = ERROR_BASE - 11,
    SET_NONBLOCKING_ERROR   = ERROR_BASE - 12,
    BIND_SOCKET_ERROR       = ERROR_BASE - 13,
    Listen_SOCKET_ERROR     = ERROR_BASE - 14,
    ACCEPT_SOCKET_ERROR     = ERROR_BASE - 15,
    EPOLL_CREATE_ERROR      = ERROR_BASE - 16,
    EPOLL_CTL_ERROR         = ERROR_BASE - 17,
};

typedef void (*tcp_recv_callback)(int, const char*, unsigned int, void *);
typedef void (*udp_recv_callback)(int, struct sockaddr_in*, const char*, unsigned int, void *);

typedef struct epoll_server{
    int epfd;
    epoll_server_type_t socket_type;
    int listen_fd;
    int stop_flag;

    tcp_recv_callback tcp_handle;
    udp_recv_callback udp_handle;
}epoll_server_t;

class EpollServer :public NetBase{
public:
    EpollServer();
    ~EpollServer();

//    int get_tcp_accept_fd();

    int create_server_fd( std::string &ip, unsigned short port, epoll_server_type_t proto );

//    epoll_server_t *epoll_tcp_server_init(unsigned short port, tcp_recv_callback handler, int max_client);
//    int epoll_tcp_server_init(unsigned short port, tcp_recv_callback handler, int max_client);
    int epoll_tcp_server_init(tcp_recv_callback handler, int max_client);


//    epoll_server_t *epoll_udp_server_init(unsigned short port, udp_recv_callback handler, int max_client);
    int epoll_udp_server_init(udp_recv_callback handler, int max_client);


    void epoll_server_start();
    void epoll_server_delete();
    void epoll_server_stop();

    void server_udp_handle(epoll_server_t* server);
    void server_tcp_client_handle(int accept_fd, epoll_server_t* server);



private:
    epoll_server_t* server_;
    int old_fd_option_;
    std::string server_ip_;
    unsigned short server_port_;
    struct sockaddr_in *server_addr_;
    struct sockaddr_in *client_addr_;

//    std::vector<struct sockaddr_in *> client_addr_list_;

    int create_tcp_socket();
    int create_udp_socket();

    int bind_socket(int fd, std::string &ip, unsigned short port, int old_fd_option);
    int listen_tcp_socket(int fd, int max);
    int accept_tcp_socket(int fd, int epfd);



};

void udp_echo_callback(int fd, struct sockaddr_in* client_addr, const char* data, unsigned int len,
                       void *reserved);

void tcp_recv_echo_callback(int fd,  const char* data, unsigned int len,
                        void *reserved);

#endif //BYS_LIB_EPOLLSERVER_H
