//
// Created by ubuntu on 6/2/21.
//

#ifndef BYS_LIB_NETBASE_H
#define BYS_LIB_NETBASE_H

#include <unistd.h>
#include <netinet/in.h>
//#include <bits/socket.h>
#include <sys/socket.h>
// 专用 本地域协议族使用 sockaddr_un
#include <sys/un.h>

// 专用 IPV4 IPV6 netinet/in.h   sockaddr_in sockaddr_in6

//  IP地址转换
#include <arpa/inet.h>

/* ====  字节序转主机序  ======
 * 1. 网络字节序转主机字节序
 *      htonl   htons
 * 2. 网络字节序转主机字节序
 *      ntohl   ntohs
 */


/* =======================   协议族    ==========================
 * 1.   协议族       地址族
 *      PF_UNIX     AF_UNIX     本地域         文件路径 108bytes
 *      sa_family_t
 *
 *      PF_INET     AF_INET     TCP/IPv4      6字节  16bit端口 32bit地址
 *
 *
 *      PF_INET6    AF_INET6    TCP/IPv6      26字节 32bit流标识 128bit地址 32bit范围ID
 */


/*  =====================   地址与字符串转换    =====================
 *  1. 点分十进制字符串转网络字节序整数IPV4      in_addr_t inet_addr(const char*strptr);
 *  2. 同上将转化结果储存在指针                int inet_aton(const char*cp,struct in_addr*inp);
 *     af地址族(AF_INET..) 成功:1 失败:0     int inet_pton(int af,const char*src,void*dst);
 *
 *  3. 网络字节序整数IPV4转点分十进制字符串     char*inet_ntoa(struct in_addr in);
 *     成功:返回目标地址 失败:NULL            const char*inet_ntop(int af,const void*src,char*dst,socklen_t cnt);
 *
 */


/*
 *  1. 创建套接字
 *      #include<sys/types.h>
 *      #nclude<sys/socket.h>
 *      int socket(int domain,int type,int protocol);
 *      domain: PF_INET4/6/UNIX
 *      type  : SOCK_NONBLOCK(非阻塞) | SOCK_CLOEXEC(fork调用子进程后关闭该socket)  [kernel>2.6.17 fcntl set]
 *      return: success fd  |   fail -1
 */

/*
 *  2. 命名套接字
 *      #include<sys/types.h>
 *      #nclude<sys/socket.h>
 *      int bind(int sockfd,const struct sockaddr*my_addr,socklen_t addrlen);
 *      sockfd      : fd
 *
 *      #include <netinet/in.h>
 *      my_addr     : sockaddr_in/in6
 *      addrlen     : sizeof(sockaddr_in/in6)
 */

/*
 *  3. 监听socket
 *      #nclude<sys/socket.h>
 *      int listen(int sockfd,int backlog);
 *      sockfd      : fd
 *      backlog     : kernel listen length      max length > backlog
 *      return      : success 0
 */

/*
 *  4.  接受连接
 *      #include<sys/types.h>
 *      #nclude<sys/socket.h>
 *      int accept(int sockfd,struct sockaddr*addr,socklen_t*addrlen);
 *      sockfd      : fd
 *
 *      #include <netinet/in.h>
 *      my_addr     : sockaddr_in/in6
 *      addrlen     : sizeof(sockaddr_in/in6)
 *
 *      只是从监听队列取出 对客户端网络断开不知情
 */

/*
 *  5.  发起连接
 *      #include<sys/types.h>
 *      #nclude<sys/socket.h>
 *      int connect(int sockfd,const struct sockaddr*serv_addr,socklen_t addrlen);
 *      sockfd      : fd
 *
 *      #include <netinet/in.h>
 *      my_addr     : sockaddr_in/in6
 *      addrlen     : sizeof(sockaddr_in/in6)
 *
 */

/*
 *  6. 关闭连接
 *      #include<unistd.h>
 *      int close(int fd);  // 非真正关闭,只是将引用计数-1
 *
 *      #include<sys/socket.h>
 *      int shutdown(int sockfd,int howto);
 *      howto   : SHUT_RD   SHUT_WR     SHUT_RDWR
 *
 */

/*
 *  7. TCP读写
 *      #include<sys/types.h>
 *      #include<sys/socket.h>
 *      ssize_t send(int sockfd,const void*buf,size_t len,int flags);
 *      flags:  MSG_CONFIRM     持续监听对方的回应 知道得到答复.用于SOCK_DGRAM/SOCK_RAW
 *              MSG_DONTROUTE   bu不查看路由表 确定目标主机在本地网络
 *              MSG_DONTWAIT    此次操作为非阻塞
 *              MSG_MORE        告诉内核还有数据要发送,内核等待新数据写入TCP一并发生,防止过小报文
 *              MSG_OOB         发送或接受紧急数据
 *              MSG_NOSIGNAL    往读端关闭的管道或socket连接写入数据时不会引发SIGPIPE信号
 *
 *      ssize_t recv(int sockfd,void*buf,size_t len,int flags);
 *      flags:  MSG_DONTWAIT    此次操作为非阻塞
 *              MSG_WAITALL     读操作在读到指定数量字节后才返回
 *              MSG_PEEK        窥视读缓存中的数据,不会导致数据被清除
 *              MSG_OOB         发送或接受紧急数据
 *
 */

/*
 *  7. UDP读写
 *      #include<sys/types.h>
 *      #include<sys/socket.h>
 *      ssize_t sendto(int sockfd,const void*buf,size_t len,int flags,const struct sockaddr*dest_addr,socklen_t addrlen);
 *      flags:  MSG_CONFIRM     持续监听对方的回应 知道得到答复.用于SOCK_DGRAM/SOCK_RAW
 *              MSG_DONTROUTE   bu不查看路由表 确定目标主机在本地网络
 *              MSG_DONTWAIT    此次操作为非阻塞
 *              MSG_MORE        告诉内核还有数据要发送,内核等待新数据写入TCP一并发生,防止过小报文
 *              MSG_OOB         发送或接受紧急数据
 *              MSG_NOSIGNAL    往读端关闭的管道或socket连接写入数据时不会引发SIGPIPE信号
 *
 *      ssize_t recvfrom(int sockfd,void*buf,size_t len,int flags,struct sockaddr*src_addr,socklen_t*addrlen);
 *      flags:  MSG_DONTWAIT    此次操作为非阻塞
 *              MSG_WAITALL     读操作在读到指定数量字节后才返回
 *              MSG_PEEK        窥视读缓存中的数据,不会导致数据被清除
 *              MSG_OOB         发送或接受紧急数据
 *
 */

/*
 *  8. 通用读写(1)
 *      #include<sys/socket.h>
 *      ssize_t sendmsg(int sockfd,struct msghdr*msg,int flags);
 *      flags:  MSG_CONFIRM     持续监听对方的回应 知道得到答复.用于SOCK_DGRAM/SOCK_RAW
 *              MSG_DONTROUTE   bu不查看路由表 确定目标主机在本地网络
 *              MSG_DONTWAIT    此次操作为非阻塞
 *              MSG_MORE        告诉内核还有数据要发送,内核等待新数据写入TCP一并发生,防止过小报文
 *              MSG_OOB         发送或接受紧急数据
 *              MSG_NOSIGNAL    往读端关闭的管道或socket连接写入数据时不会引发SIGPIPE信号
 *
 *      ssize_t ssize_t recvmsg(int sockfd,struct msghdr*msg,int flags);
 *      flags:  MSG_DONTWAIT    此次操作为非阻塞
 *              MSG_WAITALL     读操作在读到指定数量字节后才返回
 *              MSG_PEEK        窥视读缓存中的数据,不会导致数据被清除
 *              MSG_OOB         发送或接受紧急数据
 *
 *       struct msghdr {
 *              void*msg_name;         //  socket地址
 *              socklen_t msg_namelen; //  socket地址的长度
 *              struct iovec*msg_iov;  //  分散的内存块，见后文
 *              int msg_iovlen;        //  分散内存块的数量
 *              void*msg_control;      //  指向辅助数据的起始位置
 *              socklen_t msg_controllen;  //辅助数据的大小
 *              int msg_flags;         //复制函数中的flags参数，并在调用过程中更新
 *          };
 *
 *          struct iovec {
 *              void*iov_base;  //  内存起始地址
 *              size_t iov_len; //  这块内存的长度
 *           };
 *
 */

/*
 *  8. 通用读写（2）
 *  #include<sys/uio.h>
 *  ssize_t readv(int fd,const struct iovec*vector,int count);  //  分散读
 *  ssize_t writev(int fd,const struct iovec*vector,int count); //  集中写
 *  param:
 *      vector  可以以结构体数组的方式分块存储
 *      count   块数
 *
 */

/*
 *  8. 零拷贝  (1)
 *  #include<sys/sendfile.h>
 *  ssize_t sendfile(int out_fd,int in_fd,off_t*offset,size_t count);
 *  param:
 *      out_fd  准备写入的fd (必须是socket)
 *      in_fd   读出内容的fd (必须是支持mmap函数的文件描述符【指向真实文件】) 非socket pipe
 *      offset  偏移位置
 *      count   传递的长度
 */


/*
 *  8. 零拷贝  (2)     至少有一个管道文件描述符
 *  #include<fcntl.h>
 *  ssize_t splice(int fd_in,loff_t*off_in,int fd_out,loff_t*off_out,size_t len,unsigned int flags);
 *  param:
 *      fd_in   待输入数据的fd    (1)pipe ==> off_in=NULL (2)socket ==> offset
 *      fd_out  待输出数据的fd 同上
 *      len     移动数据的长度
 *      flags   控制移动方式   SPLICE_F_MOVE      kernel 2.6.21 无效
 *                           SPLICE_F_NOBLOCK   非阻塞操作 但是受文件描述符阻塞影响
 *                           SPLICE_F_MORE      后续的splice调用将读取更多的数据
 *                           SPLICE_F_GIFT      无效
 *
 *      return： success 返回移动的字节数    fail -1
 *
 *          (3)
 *  #include<fcntl.h>
 *  ssize_t tee(int fd_in,int fd_out,size_t len,unsigned int flags);
 *  param:
 *      fd_in fd_out 都必须为管道
 *
 *      return： 成功返回拷贝数量    失败返回-1
 *
 */



/*
 *  9. 外带标记
 *      #include<sys/socket.h>
 *      int sockatmark(int sockfd);
 *      return :    mark:1  no:0
 *      判断出标记就可以用(flag) MSG_OOB 来接收
 *      外带数据缓冲区与普通数据不同
 */

/*
 *  10. 地址信息函数
 *      #include<sys/socket.h>
 *      本地
 *      int getsockname(int sockfd,struct sockaddr*address,socklen_t*address_len);
 *      远端
 *      int getpeername(int sockfd,struct sockaddr*address,socklen_t*address_len);
 *      根据 fd 返回 IP PORT信息
 *
 */

/*
 *  11. socket选项
 *      #include<sys/socket.h>
 *      int getsockopt(int sockfd,int level,int option_name,void*option_value,socklen_t*restrict option_len);
 *      int setsockopt(int sockfd,int level,int option_name,const void*option_value,socklen_t option_len);
 *
 *      (1) Server: 部分选项在 listen 前针对监听socket设置才有效
 *                  有点需要在TCP同步报文段中设置,如TCP最大报文选项
 *      (2) Client: 在调用connect前设置
 *
 *      option_name:    1. SO_REUSEADDR    重用本地地址
 *      设置 buf 系统加倍 2. SO_RCVBUF       接收区大小       min:256 max:2048(depend sys)
 *                      3. SO_SNDBUF       发送区大小       /proc/sys/net/ipv4/tcp_rmem | tcp_wmem
 *                      4. SO_RCVLOWAT     超出水位线通知应用   I/O复用系统调用
 *                      5. SO_SNDLOWAT
 *                      6. SO_LINGER        控制close系统调用在关闭TCP连接时的行为
 *                          (1) l_onoff=0  SO_LINGER无效
 *                          (2) l_onoff!=0  l_linger=0 ==> close立即返回,TCP模块丢弃被关闭的socket sendbuff data,send复位
 *                                          l_linger>0 ==> 1.   阻塞close等待l_linger,直到AllSend,失败返回-1
 *                                                         2.   非阻塞立即返回
 *                      #include<sys/socket.h>
 *                      struct linger{
 *                          int l_onoff;    //  开启（非0）还是关闭（0）该选项
 *                          int l_linger;   //  滞留时间
 *                          };
 *
 */

/*
 *  12. 网络信息API
 *  (1)
 *  #include<netdb.h>
 *  struct hostent*gethostbyname(const char*name);      //  根据主机名称获取主机的完整信息
 *  struct hostent*gethostbyaddr(const void*addr,size_t len,int type);  //  数根据IP地址获取主机的完整信息
 *
 *      struct hostent{
 *           char*h_name;        //  主机名
 *           char**h_aliases;    //  主机别名列表，可能有多个
 *           int h_addrtype;     //  地址类型（地址族）
 *           char**h_addr_list;  //  按网络字节序列出的主机IP地址列表
 *      };
 *
 *  (2)
 *  struct servent*getservbyname(const char*name,const char*proto);     //  根据名称获取某个服务的完整信息
 *  struct servent*getservbyport(int port,const char*proto);            //  数根据端口号获取某个服务的完整信息
 *
 *      struct servent{
 *           char*s_name;        //  服务名称
 *           char**s_aliases;    //  服务的别名列表，可能有多个
 *           int s_port;         //  端口号
 *           char*s_proto;       //  服务类型,通常是tcp或者udp*
 *      };
 *
 *  (3)
 *  #include<netdb.h>
 *  //  能通过主机名获得IP地址 也能通过服务名获得端口号
 *  int getaddrinfo(const char*hostname,const char*service,const struct addrinfo*hints,struct addrinfo**result);
 *
 *  // 通过socket地址同时获得以字符串表示的主机名和服务名
 *  int getnameinfo(const struct sockaddr*sockaddr,socklen_t addrlen,char*host,socklen_t hostlen,
 *                  char*serv,socklen_t servlen,int flags);
 *
 *
 */

/*
 *  13. 共享内存
 *  作用：进程间通信或将文件映射到其中
 *  #include<sys/mman.h>
 *  void*mmap(void*start,size_t length,int prot,int flags,int fd,off_t offset);
 *  int munmap(void*start,size_t length);
 *
 *  param：
 *      start   特定地址作为共享内存起始地址  NULL自动分配
 *      length  内存段长度
 *      prot    访问权限：
 *              PROT_READ
 *              PROT_WRITE
 *              PROT_EXEC
 *              PROT_NONE
 *      flags   控制内存段内容被修改后程序的行为
 *              MAP_SHARED      进程间共享
 *              MAP_PRIVATE     进程私有，对该段内存修改不会反映到被映射的文件中
 *              MAP_ANONYMOUS   非文件映射产生 内容初始化为0 mmap中【fd】【offset】被忽略
 *              MAP_FIXED       内存段必须位于start参数指定地址处 start为内存页【4096】整数倍
 *              MAP_HUGETLB     按照大内存页面来分配空间    /proc/mcminfo 中查看
 *
 *  return：
 *      mmap    成功：指向目标内存区域的指针  失败：返回MAP_FAILED ((void*)-1)
 *      munmap  成功：0                   失败：-1
 */

class NetBase {
public:
    int redirect_stdout(int be_fd);
    int redirect_fd(int old_fd, int new_fd);


};


#endif //BYS_LIB_NETBASE_H
