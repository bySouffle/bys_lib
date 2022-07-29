//
// Created by bys on 2022/7/28.
//

#include "check_ip.h"
/* Include necessary libraries */
#include <arpa/inet.h>          // inet_ntop()
#include <utility>

PingFunc::PingFunc(std::string ip) : ip_(std::move(ip)), pDestinationAddr_(), ip_string_(),is_error_(1), fd_(-1) {

}

PingFunc::PingFunc() : ip_(), pDestinationAddr_(), ip_string_(),is_error_(1), fd_(-1) {

}

int PingFunc::getIPAddress(const char *hostName, addrinfo *&result) {
  addrinfo tempStruct = {0};

  tempStruct.ai_family = AF_INET;             // want IPv4
  //tempStruct.ai_socktype = SOCK_DGRAM;      // set socket type to datagram
  //tempStruct.ai_flags = AI_PASSIVE;         // fill in IP automatically

  // get and validate IP address
  return (getaddrinfo(hostName, "http", &tempStruct, &result));
}

PacketData PingFunc::initializePacket(int &transmitted) {
  PacketData packet = {};

  packet.header.type = ICMP_ECHO;                 // set ICMP type to Echo
  packet.header.un.echo.id = getpid() & 0xFFFF;   // set id (ICMP field is 16 bits)
  packet.header.checksum = 0;                     // fixed checksum because data is unchanging
  packet.header.un.echo.sequence = transmitted++;

  // fill up message
  memset(&packet.message, '0', sizeof(packet.message));
  packet.message[PacketSize - sizeof(packet.header) - 1] = '\0';

  return packet;
}

int PingFunc::sendPing(int socketFD, int &transmitted, sockaddr_in *pDestinationAddr, int &success, int &failure) {
  int bytesSent = 0, bytesReceived = 0;
  timespec StartTime, EndTime;

  // initialize packet to be sent
  PacketData packet = initializePacket(transmitted);

  // get start time
  clock_gettime(CLOCK_MONOTONIC, &StartTime);

  // send echo request
  bytesSent = sendto(socketFD,                                                            // socket file descriptor
                     (char *) &packet,
                     PacketSize,                                           // packet and size
                     0,                                                                    // flags
                     (sockaddr *) pDestinationAddr,
                     (socklen_t) sizeof(*pDestinationAddr));   // destination address and size

  if (bytesSent == -1) {
    std::cout << "Packet sending failed.\n";
    std::cout << "Error Code: " << errno << std::endl;
  } else {
    std::cout << bytesSent << " bytes sent.\n";

    // variables needed to store IP address of echo reply sender
    sockaddr_in *pReceiveFromAddr = new sockaddr_in;
    socklen_t addrLength = (socklen_t) sizeof(*pReceiveFromAddr);
    u_char receivedPacket;

    // receive echo reply
    bytesReceived = recvfrom(socketFD,                                                  // socket file descriptor
                             &receivedPacket,
                             sizeof(receivedPacket),                    // packet received and size
                             0,                                                          // flags
                             (sockaddr *) pReceiveFromAddr,
                             &addrLength);                  // address of echo reply sender and size

    if (bytesReceived == -1) {
      failure++;
      std::cout << "Packet receival failed.\n";
      std::cout << "Error Code: " << errno << std::endl;
    } else {
      success++;
      clock_gettime(CLOCK_MONOTONIC, &EndTime);

      // calculate round trip time (RTT)
      double rttTime = (EndTime.tv_sec - StartTime.tv_sec) / 1000.0 +
          (EndTime.tv_nsec - StartTime.tv_nsec) * 1000000.0;

      // convert IP address received to string
      char recvString[INET_ADDRSTRLEN];
      void *address;
      sockaddr_in *temp = pReceiveFromAddr;
      address = &temp->sin_addr;                                                  // store IP address

      // convert binary IP to string
      inet_ntop(AF_INET, address, recvString, sizeof(recvString));
      std::cout << "RECV IP: " << recvString << std::endl;

      // output details
      std::cout << sizeof(receivedPacket) + sizeof(*pReceiveFromAddr) << " bytes received from " << recvString
                << ": icmp_seq=" << transmitted << " ttl=64 time=" << rttTime << " ms\n";
    }
  }

  // avoid divide by zero error
  if (bytesSent > 0 && success > 0) {
    std::cout << (failure / (failure + success)) * 100 << "% packet loss so far.\n";
  }

  return bytesReceived;
}

int PingFunc::create_fd() {
  fd_ = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (fd_ < 0) {
    std::cout << fd_ << "Socket File Descriptor creation failed .\n";
    close(fd_);
    return -1;
  } else {
    std::cout << "Socket File Descriptor: " << fd_ << std::endl;
    return 0;
  }
}

int PingFunc::set_ttl(int fd, int ttl) {
  if (setsockopt(fd, IPPROTO_IP, IP_TTL, &TimeToLive, sizeof(TimeToLive)) != 0) {
    std::cout << "Setting Socket Options failed.\n";
    return -1;
    exit(EXIT_FAILURE);
  } else {  // for debugging purposes
    std::cout << "Socket Option Set to TTL.\n";
    return 0;
  }
}

void PingFunc::set_ip(std::string &ip) {
  ip_ = ip;
}

int PingFunc::parse_ip() {
  // get IP Address and store in 'result' (passed by reference)
  addrinfo *result = nullptr;
  if (getIPAddress(ip_.data(), result) != 0) {
    std::cout << "Invalid IP Address. Terminating ...\n";
    return -1;
    exit(EXIT_FAILURE);
  } else {
    pDestinationAddr_ = (sockaddr_in *) result->ai_addr; // get struct from resulting linked list
    void *address;
    address = &pDestinationAddr_->sin_addr; // store IP Address
    inet_ntop(result->ai_family,
              address,
              ip_string_.data(),
              sizeof(ip_string_));          // convert binary IP to string
    std::cout << "IP: " << ip_string_.data() << std::endl;
    freeaddrinfo(result);
  }

  return 0;
}

int PingFunc::set_ttl_receival(int recevial) {
  // declare and initialize struct
  timeval TimeOut = {recevial, 0};
  // set timeout of receival
  if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, (const char *) &TimeOut, sizeof(TimeOut)) != 0) {
    std::cout << "Setting Timeout for Receival failed.\n";
    return -1;
    exit(EXIT_FAILURE);
  } else {  // for debugging purposes
    std::cout << "Timeout for receival set.\n";
  }

  return 0;
}

PingFunc::~PingFunc() {
  if (thd_start_.joinable())
  thd_start_.join();
}

int PingFunc::channel_ping() {
  SetThdFlag(false);
  close(fd_);
  thd_start_.join();
  success_cnt_ = 0;
  failure_cnt_ = 0;
  is_error_ = 1;
  std::cout << "ping 已退出\n";
  return 0;
}

int PingFunc::start_ping() {
  thd_flag_ = true;
  is_error_ = 1;
  thd_start_ = std::thread(&PingFunc::routine_ping, this);
  return 0;
}

int PingFunc::config() {
  if (parse_ip() != 0) return -1;
  if (create_fd() != 0) return -2;
  if (set_ttl(fd_) != 0) return -3;
  if (set_ttl_receival(fd_) != 0) return -4;
  return 0;
}

int PingFunc::send_ping() {
  int bytesSent = 0, bytesReceived = 0;
  timespec StartTime, EndTime;

  // initialize packet to be sent
  transmitted_ = 0;
  PacketData packet = initializePacket(transmitted_);

  // get start time
  clock_gettime(CLOCK_MONOTONIC, &StartTime);

  // send echo request
  bytesSent = sendto(fd_,                                                            // socket file descriptor
                     (char *) &packet,
                     PacketSize,                                           // packet and size
                     0,                                                                    // flags
                     (sockaddr *) pDestinationAddr_,
                     (socklen_t) sizeof(*pDestinationAddr_));   // destination address and size

  if (bytesSent == -1) {
    std::cout << "Packet sending failed.\n";
    std::cout << "Error Code: " << errno << std::endl;
  } else {
    std::cout << bytesSent << " bytes sent.\n";

    // variables needed to store IP address of echo reply sender
    sockaddr_in *pReceiveFromAddr = new sockaddr_in;
    socklen_t addrLength = (socklen_t) sizeof(*pReceiveFromAddr);
    u_char receivedPacket;

    // receive echo reply
    bytesReceived = recvfrom(fd_,                                                  // socket file descriptor
                             &receivedPacket,
                             sizeof(receivedPacket),                    // packet received and size
                             0,                                                          // flags
                             (sockaddr *) pReceiveFromAddr,
                             &addrLength);                  // address of echo reply sender and size

    if (bytesReceived == -1) {
      failure_cnt_++;
      std::cout << "Packet receival failed.\n";
      std::cout << "Error Code: " << errno << std::endl;
      is_error_ = 1;
      //  todo
    } else {
      success_cnt_++;
      clock_gettime(CLOCK_MONOTONIC, &EndTime);

      // calculate round trip time (RTT)
      double rttTime = (EndTime.tv_sec - StartTime.tv_sec) / 1000.0 +
          (EndTime.tv_nsec - StartTime.tv_nsec) * 1000000.0;

      // convert IP address received to string
      char recvString[INET_ADDRSTRLEN];
      void *address;
      sockaddr_in *temp = pReceiveFromAddr;
      address = &temp->sin_addr;                                                  // store IP address

      // convert binary IP to string
      inet_ntop(AF_INET, address, recvString, sizeof(recvString));
      std::cout << "RECV IP: " << recvString << std::endl;

      is_error_ = 0;
      std::cout << "is_error " << is_error_<< "\n";
      // output details
      std::cout << sizeof(receivedPacket) + sizeof(*pReceiveFromAddr) << " bytes received from " << recvString
                << ": icmp_seq=" << transmitted_ << " ttl=64 time=" << rttTime << " ms\n";
    }
  }

  // avoid divide by zero error
  if (bytesSent > 0 && success_cnt_ > 0) {
    std::cout << "fail " << failure_cnt_ << " success " << success_cnt_ << "\n";
    std::cout << (failure_cnt_ / (failure_cnt_ + success_cnt_)) * 100 << "% packet loss so far.\n";
  }

  return bytesReceived;
}
int PingFunc::routine_ping() {
  while (true){
    int flag = send_ping();
    if (flag != -1){
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    if (thd_flag_ == false ) return 0;
  }

}
bool PingFunc::is_ping() const {
  return thd_flag_;
}

void PingFunc::SetThdFlag(bool flag) {
  thd_flag_ = flag;
}
int PingFunc::GetIsError() const {
  return is_error_;
}
