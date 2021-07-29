//
// Created by ubuntu on 7/29/21.
//

#ifndef BYS_LIB_DSP_UDPSOCKET_H
#define BYS_LIB_DSP_UDPSOCKET_H

#ifndef DSP_UDPSOCKET_H
#define DSP_UDPSOCKET_H

#include <stdio.h>

#include "string.h"
#include "stdlib.h"
#include "stdint.h"
//  dsp send pack include
//#include "driver/nimu_eth_c667x_noOS.h"

#define MAC_ERROR   (-10)
#define IP_ERROR    (-20)
#define UDP_ERROR   (-30)

//	Cpp upset
#define nullptr NULL

/* Maximum MTU size */
#define MAX_MTU         1518


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

typedef struct Dsp_SockAddr{
    uint8_t Recv_Ip[4];
    uint16_t Recv_port;
}Dsp_SockAddr_t;

#pragma pack()





/*  Use-Method
 *  1. 初始化源目    MAC        init_mac
 *  2. 初始化源     IP Port     init_src_ip_port
 *  3. 发送数据                 dsp_sendto
 *  4. 接收数据                 dsp_recvfrom
 * */


//  初始化本地 ip port
int32_t init_src_ip_port(UDP_PACKET_t *udp_packet, uint8_t src_ip[4], uint16_t port);


//  计算IP校验
uint16_t calc_ip_checksum(void * udp_packge);
//  计算UDP校验
uint16_t calc_udp_checksum(void * udp_packge, int length);
//  获取唯一ide标识
uint16_t get_only_ide(uint16_t ide);


//  ============================    mac header  ========================================
//  初始化MAC信息
//    int32_t init_mac(MAC_INFO_t *mac_info, uint8_t src_mac[6], uint8_t dst_mac[6]);
int32_t init_mac(MAC_INFO_t *mac_info, uint8_t *src_mac, uint8_t *dst_mac);
//    int32_t init_mac( uint8_t src_mac[6], uint8_t dst_mac[6]);
//	use static udp_struct
//    int32_t init_mac( uint8_t *src_mac, uint8_t *dst_mac);

//  初始化协议类型 only ipv4 0x0800
int32_t init_mac_type(MAC_INFO_t *mac_info, uint16_t type);


//  ============================    IP header   ========================================
//  初始化IP
//    int32_t init_ip_addr_(IP_INFO_t* ip_info, uint8_t src_ip[4], uint8_t dst_ip[4]);
int32_t init_ip_addr(IP_INFO_t* ip_info, uint8_t *src_ip, uint8_t *dst_ip );
int32_t init_ip_addr_i32(IP_INFO_t* ip_info, uint32_t src_ip, uint32_t dst_ip);
//  初始化 init_head_version and length
int32_t init_ip_headinfo(IP_INFO_t* ip_info, uint8_t version_length);
//  初始化dsf
int32_t init_ip_dsf(IP_INFO_t* ip_info, uint8_t DSF);
//  初始化总长度
int32_t set_ip_length(IP_INFO_t *ip_info, uint16_t len);
//  帧唯一标识 ide + 1
int32_t only_ide(IP_INFO_t* ip_info, uint16_t ide);
//  初始化flag offset
int32_t init_flag_offset(IP_INFO_t* ip_info, uint16_t param);
//  初始化TTL
int32_t init_ttl(IP_INFO_t* ip_info, uint8_t TTL);
//  初始化protocol     //UDP 0x11
int32_t init_protocol(IP_INFO_t* ip_info, uint8_t PTL);
//
int32_t set_ip_checksum(UDP_PACKET_t* udp_pack);

//  ============================    UDP header  ========================================
//  初始化端口
int32_t init_src_port(UDP_INFO_t * udp_info, uint16_t src_port);
int32_t init_dst_port(UDP_INFO_t * udp_info, uint16_t dst_port);
int16_t init_port(UDP_INFO_t * udp_info, uint16_t src_port, uint16_t dst_port);
//  初始化udp head
int32_t set_udp_length(UDP_INFO_t *udp_info, uint16_t len);
//  设置UDP校验和
int32_t set_udp_checksum(UDP_PACKET_t* udp_pack);

//  ============================    add data   ==========================================
//    inline int dsp_socket( );
int32_t dsp_add_data(UDP_PACKET_t* udp_pack, const void *data, uint16_t len);

int32_t dsp_sendto(uint8_t eMAC_Port, UDP_PACKET_t* udp_pack, uint8_t *dst_ip, uint16_t port,
                   const void *data, uint16_t len);
int32_t dsp_recvfrom(uint8_t* recv_data, Dsp_SockAddr_t* dst_ip_port, uint8_t *recv_flag);

//  ============================    make of pack    =====================================
int16_t get_pack_len(UDP_PACKET_t * udp_pack);
int16_t dsp_make_package(UDP_PACKET_t * udp_pack, uint8_t **udp_package);
int16_t dsp_unpack_package(UDP_PACKET_t * udp_pack, const uint8_t* recv_data);

#endif




#endif //BYS_LIB_DSP_UDPSOCKET_H
