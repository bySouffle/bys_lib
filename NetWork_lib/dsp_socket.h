//
// Created by ubuntu on 7/22/21.
//

#ifndef BYS_LIB_DSP_SOCKET_H
#define BYS_LIB_DSP_SOCKET_H

#include <stdint.h>
#include <string.h>
#pragma pack(4)

typedef struct MAC_INFO
{
    /* data */
    uint8_t  src_mac[6];
    uint8_t  dst_mac[6];
    uint8_t ip_version[2];
}MAC_INFO_t;

typedef struct IP_INFO
{
    /* data */
    uint8_t IP_version_len;
    uint8_t DSF;
    uint8_t total_length[2];
    uint8_t Ide[2];
    uint8_t flag_offset[2];
    uint8_t TTL_protocol[2];
    uint8_t ip_checksum[2];
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
}IP_INFO_t;

typedef struct UDP_INFO
{
    /* data */
    uint8_t src_port[2];
    uint8_t dst_port[2];
    uint8_t udp_length[2];
    uint8_t udp_checksum[2];
}UDP_INFO_t;

typedef struct UDP_PACKET
{
    /* data */
    MAC_INFO_t mac_info;
    IP_INFO_t  ip_info;
    UDP_INFO_t udp_info;
    uint8_t * data;
}UDP_PACKET_t;

#pragma pack()


class DspSocket {
public:
    DspSocket();
    ~DspSocket();
//  计算IP校验
    uint16_t calc_ip_checksum(void * udp_packge);
//  计算UDP校验
    uint16_t calc_udp_checksum(void * udp_packge, int length);


//  ============================    mac header  ========================================
//  初始化MAC信息
      int init_mac(MAC_INFO_t *mac_info, uint8_t src_mac[6], uint8_t dst_mac[6]);
      int init_mac( uint8_t src_mac[6], uint8_t dst_mac[6]);
    //  初始化协议类型 only ipv4
      int init_mac_type(MAC_INFO_t *mac_info, uint8_t type);


//  ============================    IP header   ========================================
//  初始化IP
      int init_ip_addr(IP_INFO_t* ip_info, uint8_t src_ip[4], uint8_t dst_ip[4]);
      int init_ip_addr(IP_INFO_t* ip_info, uint32_t src_ip, uint32_t dst_ip);
    //  初始化 init_head_version and length
      int init_ip_headinfo(IP_INFO_t* ip_info, uint8_t version_length);
    //  初始化dsf
      int init_ip_dsf(IP_INFO_t* ip_info, uint8_t DSF);
    //  初始化总长度
      uint16_t set_ip_length(IP_INFO_t *ip_info, uint16_t len);
    //  帧唯一标识 ide + 1
      int only_ide(IP_INFO_t* ip_info, uint16_t ide);
    //  初始化flag offset
      int init_flag_offset(IP_INFO_t* ip_info, uint16_t param);
    //  初始化TTL
      int init_ttl(IP_INFO_t* ip_info, uint8_t TTL);
    //  初始化protocol     //UDP 0x11
      int init_protocol(IP_INFO_t* ip_info, uint8_t PTL);
    //
      int set_ip_checksum(UDP_PACKET_t* udp_pack);

//  ============================    UDP header  ========================================
//  初始化端口
      int init_src_port(UDP_INFO_t * udp_info, uint16_t src_port);
      int init_dst_port(UDP_INFO_t * udp_info, uint16_t dst_port);
      int init_port(UDP_INFO_t * udp_info, uint16_t src_port, uint16_t dst_port);
//  初始化udp head
      uint16_t set_udp_length(UDP_INFO_t *udp_info, uint16_t len);
//  设置UDP校验和
      int set_udp_checksum(UDP_PACKET_t* udp_pack);

//  ============================    add data   ==========================================
//    inline int dsp_socket( );
      int dsp_adddata(UDP_PACKET_t* udp_pack, const void *data, uint16_t len);

      int dsp_sendto(const void *data, uint16_t len);

//private:
    MAC_INFO_t mac_info_;
    IP_INFO_t  ip_info_;
    UDP_INFO_t  udp_info_;
    UDP_PACKET_t udp_packet_;

};


#endif //BYS_LIB_DSP_SOCKET_H
