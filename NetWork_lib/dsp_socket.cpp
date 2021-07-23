//
// Created by ubuntu on 7/22/21.
//

#include "dsp_socket.h"
static MAC_INFO_t mac_info_;
static IP_INFO_t  ip_info_;
static UDP_PACKET_t udp_packet_;

static uint16_t ide_cnt_ = 0;

uint16_t DspSocket::calc_ip_checksum(void *udp_packge) {
    UDP_PACKET_t* m_udp_pack = (UDP_PACKET_t* )udp_packge;
    IP_INFO_t* m_ip_info = &m_udp_pack->ip_info;
    uint8_t index = 0;
    uint32_t ip_checksum = 0;
    uint16_t pro_data = 0;
    uint16_t rst_checksum = 0;
    switch (index) {
        case 0:
            ip_checksum += ( (m_ip_info->IP_version_len << 8) & 0xff00 ) &
                           ( (m_ip_info->DSF) & 0xff );
            index++;
        case 1:
            ip_checksum += ( (m_ip_info->total_length[0] << 8)&0xff00 ) &
                           ( (m_ip_info->total_length[1]) & 0xff );
            index++;
        case 2:
            ip_checksum += ( (m_ip_info->Ide[0] << 8)&0xff00 ) &
                        ( (m_ip_info->Ide[1]) & 0xff );
            index++;
        case 3:
            ip_checksum += ( (m_ip_info->flag_offset[0] << 8)&0xff00 ) &
                        ( (m_ip_info->flag_offset[1]) & 0xff );
            index++;
        case 4:
            ip_checksum += ( (m_ip_info->TTL_protocol[0] << 8)&0xff00 ) &
                        ( (m_ip_info->TTL_protocol[1]) & 0xff );
            index++;
        case 5:
            ip_checksum += ( (m_ip_info->src_ip[0] << 8)&0xff00 ) &
                        ( (m_ip_info->src_ip[1]) & 0xff );
            ip_checksum += ( (m_ip_info->src_ip[2] << 8)&0xff00 ) &
                        ( (m_ip_info->src_ip[3]) & 0xff );
            index++;
        case 6:
            ip_checksum += ( (m_ip_info->dst_ip[0] << 8)&0xff00 ) &
                        ( (m_ip_info->dst_ip[1]) & 0xff );
            ip_checksum += ( (m_ip_info->dst_ip[2] << 8)&0xff00 ) &
                        ( (m_ip_info->dst_ip[3]) & 0xff );
            index++;
        default:
            break;
    }

    rst_checksum = (ip_checksum >> 16)+(ip_checksum&0xffff);
    rst_checksum += (rst_checksum >> 16);

    return rst_checksum;
}


uint16_t DspSocket::calc_udp_checksum(void *udp_packge) {
    UDP_PACKET_t* m_udp_pack = (UDP_PACKET_t* )udp_packge;
    IP_INFO_t* m_ip_info = &m_udp_pack->ip_info;
    UDP_INFO_t* m_udp_info = &m_udp_pack->udp_info;

    uint8_t index = 0;
    uint32_t ip_checksum = 0;
    uint16_t pro_data = 0;
    uint16_t rst_checksum = 0;

    switch (index) {
        case 1:
            m_ip_info.


    }
    return 0;
}

int DspSocket::init_mac(MAC_INFO_t *mac_info, uint8_t *src_mac, uint8_t *dst_mac) {
    memset(mac_info, 0, sizeof(MAC_INFO_t));
    memcpy(mac_info->src_mac, src_mac, sizeof (mac_info_.src_mac));
    memcpy(mac_info->src_mac, dst_mac, sizeof (mac_info_.dst_mac));
    return 0;
}

int DspSocket::init_mac(uint8_t *src_mac, uint8_t *dst_mac) {
    memset(&mac_info_, 0, sizeof(MAC_INFO_t));
    memcpy(&mac_info_.src_mac, src_mac, sizeof (mac_info_.src_mac));
    memcpy(&mac_info_.dst_mac, dst_mac, sizeof (mac_info_.dst_mac));
    return 0;
}

int DspSocket::init_ip_addr(IP_INFO_t *ip_info, uint8_t *src_ip, uint8_t *dst_ip) {
    memset(ip_info, 0, sizeof(IP_INFO_t));
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
    if(udp_info!= nullptr){
        return  -1;
    }
    udp_info->src_port[0] = (src_port >> 8)&0xFF;
    udp_info->src_port[1] = (src_port)&0xFF;
    return 0;
}

int DspSocket::init_dst_port(UDP_INFO_t *udp_info, uint16_t dst_port) {
    if(udp_info== nullptr){
        return  -1;
    }
    udp_info->src_port[0] = (dst_port >> 8)&0xFF;
    udp_info->src_port[1] = (dst_port)&0xFF;
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

int DspSocket::only_ide(IP_INFO_t *ip_info) {
    ide_cnt_++;
    ip_info->Ide[0] = (ide_cnt_ >> 8)&0xFF;
    ip_info->Ide[1] = (ide_cnt_     )&0xFF;
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

uint16_t DspSocket::set_udp_length(IP_INFO_t *ip_info, uint16_t len) {
    ip_info->total_length[0] = (len >> 8)&0xFF;
    ip_info->total_length[1] = (len     )&0xFF;
    return 0;
}

int DspSocket::dsp_sendto(const void *data, uint16_t len) {
    UDP_PACKET_t udp_packet;
    memset(&udp_packet,0,sizeof (UDP_PACKET_t));
    memcpy(&udp_packet.mac_info, &mac_info_, sizeof (MAC_INFO_t));
    memcpy(&udp_packet.ip_info, &ip_info_, sizeof (IP_INFO_t));
    memcpy(&udp_packet.udp_info, &udp_packet_, sizeof (UDP_INFO_t));
    memcpy(&udp_packet.data, data, len);

    calc_ip_checksum(&udp_packet);
    calc_udp_checksum(&udp_packet);
    return 0;
}













