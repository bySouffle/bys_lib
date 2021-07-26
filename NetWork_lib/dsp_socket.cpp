//
// Created by ubuntu on 7/22/21.
//
#include "stdlib.h"
#include "stdio.h"
#include "dsp_socket.h"
//static MAC_INFO_t mac_info_;
//static IP_INFO_t  ip_info_;
//static UDP_PACKET_t udp_packet_;

static uint16_t ide_cnt_ = 0;

uint16_t DspSocket::calc_ip_checksum(void *udp_packge) {
    UDP_PACKET_t* m_udp_pack = (UDP_PACKET_t* )udp_packge;
    IP_INFO_t* m_ip_info = &m_udp_pack->ip_info;
    uint8_t index = 0;
    uint32_t ip_checksum = 0;
    uint16_t pro_data = 0;
    uint16_t rst_checksum = 0;

    uint8_t a = 0x64;
    uint8_t b = 0x65;

    switch (index) {
        case 0:
            pro_data = ( (m_ip_info->IP_version_len << 8)&0xFF00) |
                    (m_ip_info->DSF &0xFF);
            ip_checksum += ( (m_ip_info->IP_version_len << 8) & 0xff00 ) |
                           ( (m_ip_info->DSF) & 0xff );
            index++;
        case 1:
            ip_checksum += ( (m_ip_info->total_length[0] << 8)&0xff00 ) |
                           ( (m_ip_info->total_length[1]) & 0xff );
            index++;
        case 2:
            ip_checksum += ( (m_ip_info->Ide[0] << 8)&0xff00 ) |
                           ( (m_ip_info->Ide[1]) & 0xff );
            index++;
        case 3:
            ip_checksum += ( (m_ip_info->flag_offset[0] << 8)&0xff00 ) |
                           ( (m_ip_info->flag_offset[1]) & 0xff );
            index++;
        case 4:
            ip_checksum += ( (m_ip_info->TTL_protocol[0] << 8)&0xff00 ) |
                        ( (m_ip_info->TTL_protocol[1]) & 0xff );
            index++;
        case 5:
            ip_checksum += ( (m_ip_info->src_ip[0] << 8)&0xff00 ) |
                        ( (m_ip_info->src_ip[1]) & 0xff );
            ip_checksum += ( (m_ip_info->src_ip[2] << 8)&0xff00 ) |
                        ( (m_ip_info->src_ip[3]) & 0xff );
            index++;
        case 6:
            ip_checksum += ( (m_ip_info->dst_ip[0] << 8)&0xff00 ) |
                        ( (m_ip_info->dst_ip[1]) & 0xff );
            ip_checksum += ( (m_ip_info->dst_ip[2] << 8)&0xff00 ) |
                        ( (m_ip_info->dst_ip[3]) & 0xff );
            index++;
        default:
            break;
    }

    ip_checksum = (ip_checksum >> 16)+(ip_checksum&0xffff);
    ip_checksum += (ip_checksum >> 16);
    ip_checksum = 0xffff - ip_checksum;
    return ip_checksum;
}


uint16_t DspSocket::calc_udp_checksum(void *udp_packge, int length) {
    UDP_PACKET_t* m_udp_pack = (UDP_PACKET_t* )udp_packge;
    IP_INFO_t* m_ip_info = &m_udp_pack->ip_info;
    UDP_INFO_t* m_udp_info = &m_udp_pack->udp_info;

    uint8_t index = 0;
    uint32_t udp_checksum = 0;
    uint16_t pro_data = 0;
    uint16_t rst_checksum = 0;
    int i = 0;

    switch (index) {
        case 0:
            udp_checksum += ( (m_ip_info->src_ip[0] << 8)&0xff00 ) |
                            ( (m_ip_info->src_ip[1]) & 0xff );
            udp_checksum += ( (m_ip_info->src_ip[2] << 8)&0xff00 ) |
                            ( (m_ip_info->src_ip[3]) & 0xff );
            index ++;
        case 1:
            udp_checksum += ( (m_ip_info->dst_ip[0] << 8)&0xff00 ) |
                            ( (m_ip_info->dst_ip[1]) & 0xff );
            udp_checksum += ( (m_ip_info->dst_ip[2] << 8)&0xff00 ) |
                            ( (m_ip_info->dst_ip[3]) & 0xff );
            index ++;
        case 2:
            udp_checksum += ( (m_ip_info->TTL_protocol[1] &0xFF) );
            udp_checksum += ( (m_udp_info->udp_length[0] <<8)&0xFF00) |
                            ( (m_udp_info->udp_length[1]) & 0xFF );
            index ++;
        case 3:
            udp_checksum += ( (m_udp_info->src_port[0] << 8)& 0xFF00 ) |
                            ( (m_udp_info->src_port[1])&0xFF );
            udp_checksum += ( (m_udp_info->dst_port[0] << 8)& 0xFF00 ) |
                            ( (m_udp_info->dst_port[1])&0xFF );
            udp_checksum += ( (m_udp_info->udp_length[0] << 8)& 0xFF00 ) |
                            ( (m_udp_info->udp_length[1])&0xFF );
            index ++;
        case 4:
            //  TODO limit
            if(length%2 == 0){
                for (i = 0; i < length -1; i+=2) {

                    udp_checksum += ( ((m_udp_pack->data[i] &0xFF ) <<8)&0xFF00) |
                                    ( (m_udp_pack->data[i+1])&0xFF );
                }
                //   even padding 0x00 at the last
                udp_checksum += ((m_udp_pack->data[length]&0xFF) << 8)&0xFF00 | 0x00;
            } else{
                for ( i = 0; i < length; i+=2) {
//                while (i<length){
                    udp_checksum += ( ((m_udp_pack->data[i] &0xFF ) <<8)&0xFF00) |
                                    ( (m_udp_pack->data[i+1])&0xFF );
//                i+=2;
                }
            }

        default:
            break;
    }
    udp_checksum = (udp_checksum >> 16)+(udp_checksum&0xffff);
    udp_checksum += (udp_checksum >> 16);
    udp_checksum = 0xFFFF - udp_checksum;
    return udp_checksum;
}

int DspSocket::init_mac(MAC_INFO_t *mac_info, uint8_t *src_mac, uint8_t *dst_mac) {
    memset(mac_info, 0, sizeof(MAC_INFO_t));
    memcpy(&mac_info->src_mac, src_mac, sizeof (mac_info_.src_mac));
    memcpy(&mac_info->dst_mac, dst_mac, sizeof (mac_info_.dst_mac));
    return 0;
}

int DspSocket::init_mac(uint8_t *src_mac, uint8_t *dst_mac) {
    memset(&mac_info_, 0, sizeof(MAC_INFO_t));
    memcpy(&mac_info_.src_mac, src_mac, sizeof (mac_info_.src_mac));
    memcpy(&mac_info_.dst_mac, dst_mac, sizeof (mac_info_.dst_mac));
    return 0;
}

int DspSocket::init_ip_addr(IP_INFO_t *ip_info, uint8_t *src_ip, uint8_t *dst_ip) {
//    memset(ip_info, 0, sizeof(IP_INFO_t));
    memcpy(ip_info->src_ip, src_ip, sizeof (ip_info->src_ip));
    memcpy(ip_info->dst_ip, dst_ip, sizeof (ip_info->dst_ip));
    return 0;
}

int DspSocket::init_ip_addr(IP_INFO_t *ip_info, uint32_t src_ip, uint32_t dst_ip) {
    memset(&ip_info_, 0, sizeof(IP_INFO_t));
    uint8_t index;
    for ( index = 0; index < 4; ++index) {
        ip_info->src_ip[index] = src_ip >> (12-4*index) & 0xFF;
        ip_info->dst_ip[index] = dst_ip >> (12-4*index) & 0xFF;
    }
    return 0;
}

int DspSocket::init_src_port(UDP_INFO_t *udp_info, uint16_t src_port) {
    if(udp_info == nullptr){
        return  -1;
    }
    udp_info->src_port[0] = (src_port >> 8)&0xFF;
    udp_info->src_port[1] = (src_port)&0xFF;
    return 0;
}

int DspSocket::init_dst_port(UDP_INFO_t *udp_info, uint16_t dst_port) {
    if(udp_info == nullptr){
        return  -1;
    }
    udp_info->dst_port[0] = (dst_port >> 8)&0xFF;
    udp_info->dst_port[1] = (dst_port)&0xFF;
    return 0;
}

int DspSocket::init_port(UDP_INFO_t * udp_info, uint16_t src_port, uint16_t dst_port) {
    init_dst_port(udp_info, dst_port);
    init_src_port(udp_info, src_port);
    return 0;
}

int DspSocket::init_mac_type(MAC_INFO_t *mac_info, uint8_t type) {
    //  TODO    if use others
    mac_info->ip_version[0] = 0x08;
    mac_info->ip_version[1] = 0x00;
    return 0;
}

int DspSocket::init_ip_headinfo(IP_INFO_t *ip_info, uint8_t version_length) {
    //  TODO if use others
    ip_info->IP_version_len= 0x45;

    return 0;
}

int DspSocket::init_ip_dsf(IP_INFO_t *ip_info, uint8_t DSF) {
    ip_info->DSF = 0x00;
    return 0;
}

int DspSocket::only_ide(IP_INFO_t *ip_info, uint16_t ide) {
    if(ide!=0){
        ip_info->Ide[0] = ((ide&0xFF00) >> 8);
        ip_info->Ide[1] = (ide&0xff);
    } else{
        ide_cnt_++;
        ip_info->Ide[0] = (ide_cnt_ >> 8)&0xFF;
        ip_info->Ide[1] = (ide_cnt_     )&0xFF;
    }
    return 0;
}

int DspSocket::init_flag_offset(IP_INFO_t *ip_info, uint16_t param) {
    //  TODO if set others
    ip_info->flag_offset[0] = 0x00;
    ip_info->flag_offset[1] = 0x00;
    return 0;
}

int DspSocket::init_ttl(IP_INFO_t *ip_info, uint8_t TTL) {
    ip_info->TTL_protocol[0] = TTL;
    return 0;
}

int DspSocket::init_protocol(IP_INFO_t *ip_info, uint8_t PTL) {
    ip_info->TTL_protocol[1] = PTL;
    return 0;
}

uint16_t DspSocket::set_ip_length(IP_INFO_t *ip_info, uint16_t len) {
    ip_info->total_length[0] = (len >> 8)&0xFF;
    ip_info->total_length[1] = (len     )&0xFF;
    return 0;
}

//int DspSocket::dsp_sendto(const void *data, uint16_t len) {
//    UDP_PACKET_t udp_packet;
//    memset(&udp_packet,0,sizeof (UDP_PACKET_t));
//    memcpy(&udp_packet.mac_info, &mac_info_, sizeof (MAC_INFO_t));
//    memcpy(&udp_packet.ip_info, &ip_info_, sizeof (IP_INFO_t));
//    memcpy(&udp_packet.udp_info, &udp_packet_, sizeof (UDP_INFO_t));
//    memcpy(&udp_packet.data, data, len);
//
//    calc_ip_checksum(&udp_packet);
//    //  TODO SEND
//    calc_udp_checksum(&udp_packet, );
//    return 0;
//}

uint16_t DspSocket::set_udp_length(UDP_INFO_t *udp_info, uint16_t len) {
    udp_info->udp_length[0] = (len >> 8)&0xFF;
    udp_info->udp_length[1] = (len     )&0xFF;
    return 0;
}

int DspSocket::dsp_adddata(UDP_PACKET_t* udp_pack, const void *data, uint16_t len) {
    uint8_t *udp_data = (uint8_t *)malloc(len*sizeof (uint8_t));
    //  TODO
    udp_pack->data = (uint8_t *)malloc(len*sizeof (uint8_t));
    memcpy(udp_pack->data, data, len);
    uint16_t udp_len = len+8;   //  udp head length
    set_udp_length(&udp_pack->udp_info,udp_len);
    udp_len += 20;              //  total length
    set_ip_length(&udp_pack->ip_info,udp_len);

    //  TODO calc checksum

    uint16_t ip_sum = calc_ip_checksum(udp_pack);
    printf("ip_sum: %d \r\n", ip_sum);
    uint16_t udp_sum = calc_udp_checksum(udp_pack,udp_len-28);
    printf("udp_sum: %d \r\n", udp_sum);

    return 0;
}

DspSocket::DspSocket() {
     mac_info_ = {};
      ip_info_ = {};
      udp_info_ = {};
     udp_packet_ = {};

}

DspSocket::~DspSocket() {

}

int DspSocket::set_ip_checksum(UDP_PACKET_t* udp_pack) {
    uint16_t ip_checksum = calc_ip_checksum(udp_pack);
    udp_pack->ip_info.ip_checksum[0] = (ip_checksum >> 8)&0xff;
    udp_pack->ip_info.ip_checksum[1] = (ip_checksum     )&0xff;
    return 0;
}

int DspSocket::set_udp_checksum(UDP_PACKET_t *udp_pack) {
    uint16_t len = (udp_pack->udp_info.udp_length[0]<<8) |
                    udp_pack->udp_info.udp_length[1];
    uint16_t udp_checksum = calc_udp_checksum(udp_pack, len);
    udp_pack->udp_info.udp_checksum[0] = (udp_checksum >> 8)&0xff;
    udp_pack->udp_info.udp_checksum[1] = (udp_checksum     )&0xff;

    return 0;
}














