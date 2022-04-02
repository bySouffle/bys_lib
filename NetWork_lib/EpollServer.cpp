//
// Created by souffle on 8/31/21.
//

#include "EpollServer.h"

EpollServer::EpollServer() :
    server_(new epoll_server_t), server_addr_(new struct sockaddr_in), client_addr_(new struct sockaddr_in),
    server_ip_("192.168.179.208"), server_port_(50010)
//    server_ip_("127.0.0.1"), server_port_(60000)

{
    memset(server_,0,sizeof (epoll_server_t));
    memset(server_addr_,0,sizeof (struct sockaddr_in));
    memset(client_addr_,0,sizeof (struct sockaddr_in));

}

EpollServer::~EpollServer() {
    delete server_;
    delete server_addr_;
    delete client_addr_;

//    for (int i = 0; i < client_addr_list_.size(); ++i) {
//        delete client_addr_list_.at(i);
//    }

}

int EpollServer::epoll_tcp_server_init(tcp_recv_callback handler, int max_client) {
    struct epoll_event ev;
    server_->epfd = epoll_create(kMaxEpollSize);
    if( server_->epfd < 0 )     {   return EPOLL_CREATE_ERROR;    };
    server_->socket_type = TCP_PROTO;
    server_->tcp_handle = handler;
    server_->listen_fd = create_server_fd(server_ip_, server_port_, server_->socket_type);
    if( server_->listen_fd < 0 )    {   return server_->listen_fd;    };

    ev.events = EPOLLIN;
    ev.data.fd = server_->listen_fd;
    int ret_val = epoll_ctl(server_->epfd,EPOLL_CTL_ADD, server_->listen_fd, &ev);
    if( ret_val < 0 ){
        return EPOLL_CTL_ERROR;
    }

    return 0;
}

int EpollServer::create_tcp_socket() {
    int fd = socket(AF_INET, SOCK_STREAM, 0 );
    if(fd < 0){
        return CREATE_TCP_SOCKET_ERROR;
    }
    return fd;
}

int EpollServer::create_udp_socket() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if(fd < 0){
        return CREATE_UDP_SOCKET_ERROR;
    }
    return fd;
}

int EpollServer::bind_socket(int fd, std::string &server_ip, unsigned short port, int old_fd_option) {
    server_addr_->sin_family = AF_INET;
//    server_addr_->sin_addr.s_addr = inet_addr(server_ip.data());
    inet_pton(AF_INET,server_ip.data(),&server_addr_->sin_addr);
    server_addr_->sin_port = htons(port);
    socklen_t addr_len = sizeof (struct sockaddr);
    int ret_val ;
//    ret_val = set_socket_nonblocking(fd);
//    if( ret_val < 0 ) {     return SET_NONBLOCKING_ERROR;    }
    old_fd_option = ret_val;

    ret_val = bind(fd, (struct sockaddr*)server_addr_, addr_len);
    if( ret_val < 0 ) {     return  BIND_SOCKET_ERROR;      };

    return 0;
}

int EpollServer::listen_tcp_socket(int fd, int max) {
    int ret_val = listen(fd, max);
    if( ret_val < 0 )   {     return Listen_SOCKET_ERROR;      };
    return 0;
}

int EpollServer::accept_tcp_socket(int fd, int epfd) {
    int client_fd;
    struct epoll_event ev;

    struct sockaddr_in* client_addr = new struct sockaddr_in;
    socklen_t sockaddr_len = sizeof (struct sockaddr_in);
    memset(client_addr, 0, sizeof(struct sockaddr_in));
    client_fd = accept(fd, (struct sockaddr*)client_addr, &sockaddr_len );
    if( client_fd < 0 ) {      return ACCEPT_SOCKET_ERROR;     }

    int ret_val = set_socket_nonblocking(client_fd);
    if( ret_val < 0 ) {     return SET_NONBLOCKING_ERROR;    }

//    client_addr_list_.push_back(client_addr);

    ev.events = EPOLLIN;
    ev.data.fd = client_fd;

    epoll_ctl( epfd, EPOLL_CTL_ADD, client_fd, &ev );
    return 0;
}

int EpollServer::create_server_fd(std::string &ip, unsigned short port, epoll_server_type_t proto) {
    int fd = -1;
    if(proto == TCP_PROTO){
        fd = create_tcp_socket();
        if(fd < 0)   return fd ;
        int ret_val = bind_socket(fd, server_ip_, server_port_, old_fd_option_);
        if( ret_val < 0 )    return ret_val;
        ret_val = listen_tcp_socket(fd, kMaxListen);
        if( ret_val < 0 )    return ret_val;

        //  TODO    TEST
        char recv_buf[1024] = {};
        struct sockaddr_in client_addr1 = {};
        socklen_t addr_len = sizeof (sockaddr);
        int cl_fd = 0;

#if 0
        while (1){
            cl_fd = accept(fd, (struct sockaddr*)&client_addr1,&addr_len);
            if(cl_fd > 0) {
                while (1) {
                    recv(cl_fd, recv_buf, 100, 0);
                    print("data: %s\r\n", recv_buf);
                    send(cl_fd, recv_buf, 100, 0);
                    memset(recv_buf, 0, sizeof(recv_buf));
                    close(cl_fd);
                    cl_fd = 0;
                    break;
                }
            }
        }
#endif

    } else if(proto == UDP_PROTO){
        fd = create_udp_socket();
        if(fd < 0)   return fd ;
        int ret_val = bind_socket(fd, server_ip_, server_port_, old_fd_option_);
        if( ret_val < 0 )    return ret_val;

//        server_->listen_fd = fd;
        //  TODO    TEST
#if 0
        char recv_buf[1024] = {};
        struct sockaddr_in client_addr1 = {};
        socklen_t addr_len = sizeof (sockaddr);
        while (1){
            int len = recvfrom(server_->listen_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr1, &addr_len);
            if(len > 0){
                print("data: %s\r\n", recv_buf);
//                server_->udp_handle(server_->listen_fd, client_addr_, recv_buf, len, nullptr);
                sendto(fd, recv_buf, len, 0, (struct sockaddr* )&client_addr1, sizeof(struct sockaddr_in));

            }

        }
#endif



    }
        return fd;

}

void EpollServer::epoll_server_start() {
    int ready;
    struct epoll_event events[kMaxEpollSize];

    while (true){
        if(server_->stop_flag){
            printf("server STOP\r\n");
            break;
        }
        ready = epoll_wait(server_->epfd,events,kMaxEpollSize, 10);
        if(ready < 0){
            perror("epoll_wait fail\r\n");
            return;
        } else if (ready == 0){
            //  timeout no ready
//            print("epoll_wait\r\n");
            continue;
        } else{
            for(int i = 0; i < ready; ++i){
                if (events[i].data.fd == server_->listen_fd){
                    if (server_->socket_type == TCP_PROTO){
                        accept_tcp_socket(server_->listen_fd, server_->epfd);
                    } else{
                        //  udp handle
                        server_udp_handle(server_);
                    }
                } else{
                    //  TCP 接收客户端处理
                    server_tcp_client_handle(events[i].data.fd, server_);
                }
            }
        }
    }

}

//int EpollServer::get_tcp_accept_fd() {
//    return 0;
//}

void EpollServer::epoll_server_delete() {
    if(server_  != nullptr){
        close(server_->epfd);
    } else{
        return;
    }
}

void EpollServer::epoll_server_stop() {
    server_->stop_flag = 1;
}

void EpollServer::server_udp_handle(epoll_server_t *server) {
    char recv_buf[65536] = {};
    int len;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(struct sockaddr);

    len = recvfrom(server->listen_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &client_len);
    if(len > 0){
        server->udp_handle(server->listen_fd, &client_addr, recv_buf, len, nullptr);
    }
}

void EpollServer::server_tcp_client_handle(int accept_fd, epoll_server_t *server) {
    ScalableBuff scalable_buff(KBUFF_SIZE);
    char recv_buf[KBUFF_SIZE] = {};
    int len;
    struct epoll_event ev;
    //  TODO send client
    send(accept_fd,"zzzzzz",7,0);

    while ( (len = recv(accept_fd, recv_buf, sizeof (recv_buf), 0)) > 0) {
        scalable_buff.push_vecbuf(recv_buf,len);
        Print("SERVER RECV DATA: %s\r\n", scalable_buff.get_data());
    }

    if (len == 0){
        ev.data.fd = accept_fd;
        epoll_ctl(server_->epfd, EPOLL_CTL_DEL, accept_fd, &ev);
        close(accept_fd);
    } else if (len < 0 && errno != EAGAIN){
        Print("recv-error\r\n");
    }

    if(scalable_buff.get_used() > 0){
        server_->tcp_handle(accept_fd, scalable_buff.get_data(), scalable_buff.get_used(), nullptr);
    }
}

int EpollServer::epoll_udp_server_init( udp_recv_callback handler, int max_client) {
    struct epoll_event ev;
    server_->epfd = epoll_create(kMaxEpollSize);
    if( server_->epfd < 0 )     {   return EPOLL_CREATE_ERROR;    };
    server_->socket_type = UDP_PROTO;
    server_->udp_handle = handler;
    server_->listen_fd = create_server_fd(server_ip_, server_port_, server_->socket_type);
    if( server_->listen_fd < 0 )    {   return server_->listen_fd;    };

    ev.events = EPOLLIN;
    ev.data.fd = server_->listen_fd;
    int ret_val = epoll_ctl(server_->epfd,EPOLL_CTL_ADD, server_->listen_fd, &ev);
    if( ret_val < 0 ){
        return EPOLL_CTL_ERROR;
    }

    return 0;
}

void udp_echo_callback(int fd, struct sockaddr_in *client_addr, const char *data, unsigned int len,
                                    void *reserved) {
    Print(">> client addr = %s:%d\r\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    printf("Recv: %s\r\n", data);
    sendto(fd, data, len, 0, (struct sockaddr* )client_addr, sizeof(struct sockaddr_in));
}

void tcp_recv_echo_callback(int fd, const char *data, unsigned int len, void *reserved) {
    send(fd, data, len, 0);
}

//void recv_echo_callback(int fd, struct sockaddr_in *client_addr, const char *data, unsigned int len,
//                                     void *reserved) {
//    send(fd, data, len, 0);
//}


