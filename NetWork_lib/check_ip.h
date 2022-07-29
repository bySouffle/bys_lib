//
// Created by bys on 2022/7/28.
//

#ifndef BYS_LIB_NETWORK_LIB_CHECK_IP_H_
#define BYS_LIB_NETWORK_LIB_CHECK_IP_H_


/* Include necessary libraries */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>              // getaddrinfo()
#include <netinet/ip.h>         // for n_short and n_time for use in netinet/ip_icmp.h
#include <netinet/ip_icmp.h>    // icmp header
#include <unistd.h>             // getpid()
#include <time.h>
//--//
#include <stdlib.h>
#include <string.h>             // memset()
//--//
#include <iostream>
#include <string>
#include <array>
#include <thread>
#include <atomic>
/** @struct PacketData
 *  @brief  Structure to hold packet to be sent. Includes icmp header and the message.
 */
struct PacketData {
  icmphdr header;
  char message[(64 - sizeof(header))];      // want total size to be 64 bytes
};

class PingFunc {
 public:
  PingFunc();
  PingFunc(std::string ip);

  //! 设置ip
  //! \param ip
  //! \return
  void set_ip(std::string &ip);

  int channel_ping();

  int start_ping();

  //! 完成所有配置
  //! \return success 0
  int config();

  ~PingFunc();
 private:
  std::string ip_;  //  ping ip
  sockaddr_in *pDestinationAddr_;
  std::array<char, 16> ip_string_;

  volatile int is_error_;
 public:
  int GetIsError() const;
 private:
  //  错误标志

  int success_cnt_ = 0; //  ping 成功计数
  int failure_cnt_ = 0; //  ping 失败计数
  int fd_;  //  文件描述符
  int transmitted_;

  std::atomic_bool thd_flag_;
 public:
  bool is_ping() const;

 private:
  //  正在ping  线程退出标志

  //! 创建fd
  //! \return -1  err
  int create_fd();

  //! 设置ttl
  //! \param ttl 默认 64
  //! \return -1 err
  int set_ttl(int fd, int ttl = 64);

  //! 设置recevial
  //! \param recevial 默认 3
  //! \return
  int set_ttl_receival(int recevial = 3);

  //! 解析ip
  //! \return
  int parse_ip();

  std::thread thd_start_;

 private:
/* Define constants */
  const int PacketSize = 64;      // packet size to be kept as 64 Bytes
  const int ReceivalTimeOut = 3;  // timeout delay for receiving packets (3 seconds)
  const int TimeToLive = 64;      // ttl value


/** @fn     getIPAddress(char*, addrinfo*&)
 *  @brief  Obtain IP Address in addrinfo from domain name or IP Address stored as char.
 */
  int getIPAddress(const char *hostName, addrinfo *&result);
//  int getIPAddress(char* hostName, addrinfo &result);

/** @fn     initializePacket(int&)
 *  @brief  Set up packet to be sent accordingly for ICMP.
 */
  PacketData initializePacket(int &transmitted);

/** @fn     sendPing()
 *  @brief  Function to send an ICMP echo request and receive an echo reply. Makes use of the
 *          initializePacket() function to create a packet and sends to the address pointed to
 *          by pDestinationAddr (a pointer). Displays RTT (Round Trip Time in milliseconds)
 *          and packet success percentage after every ping.
 */
  int sendPing(int socketFD, int &transmitted, sockaddr_in *pDestinationAddr, int &success, int &failure);

  int send_ping();

  int routine_ping();

  void SetThdFlag(bool flag);

};

#endif //BYS_LIB_NETWORK_LIB_CHECK_IP_H_
