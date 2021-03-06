//
// Created by ubuntu on 7/29/21.
//

#include "dsp_udpsocket.h"
/*
 * dsp_udpsocket.c
 *
 *  Created on: 2021-7-29
 *      Author: bysou
 */

#include "dsp_udpsocket.h"

//	UDP IDE flag count
static uint16_t ide_cnt_ = 0;

//	DSP recv package
static uint8_t dsp_recv_data[MAX_MTU];
//	DSP recv flags RX
//static uint8_t recv_flag;


uint16_t calc_ip_checksum(void *udp_packge) {
    UDP_PACKET_t* m_udp_pack = (UDP_PACKET_t* )udp_packge;
    IP_INFO_t* m_ip_info = &m_udp_pack->ip_info;
    uint8_t index = 0;
    uint32_t ip_checksum = 0;


    switch (index) {
        case 0:
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
    return ip_checksum & 0xffff;
}


uint16_t calc_udp_checksum(void *udp_packge, int length) {
    UDP_PACKET_t* m_udp_pack = (UDP_PACKET_t* )udp_packge;
    IP_INFO_t* m_ip_info = &m_udp_pack->ip_info;
    UDP_INFO_t* m_udp_info = &m_udp_pack->udp_info;

    uint8_t index = 0;
    uint32_t udp_checksum = 0;

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

int32_t init_mac(MAC_INFO_t *mac_info, uint8_t *src_mac, uint8_t *dst_mac) {
    memset(mac_info, 0, sizeof(MAC_INFO_t));
    memcpy(&mac_info->src_mac, src_mac, sizeof (mac_info->src_mac));
    memcpy(&mac_info->dst_mac, dst_mac, sizeof (mac_info->dst_mac));
    return 0;
}

/*
int32_t init_mac(uint8_t *src_mac, uint8_t *dst_mac) {
    memset(&udp_packet_.mac_info, 0, sizeof(MAC_INFO_t));
    memcpy(&udp_packet_.mac_info.src_mac, src_mac, sizeof (udp_packet_.mac_info.src_mac));
    memcpy(&udp_packet_.mac_info.dst_mac, dst_mac, sizeof (udp_packet_.mac_info.dst_mac));
    return 0;
}
*/

int32_t init_ip_addr(IP_INFO_t *ip_info, uint8_t *src_ip, uint8_t *dst_ip) {
//    memset(ip_info, 0, sizeof(IP_INFO_t));
    if(ip_info == nullptr){
        return -1;
    }
    if(src_ip != nullptr){
        memcpy(ip_info->src_ip, src_ip, sizeof (ip_info->src_ip));
    }
    if(dst_ip != nullptr){
        memcpy(ip_info->dst_ip, dst_ip, sizeof (ip_info->dst_ip));
    }
    return 0;
}

int32_t init_ip_addr_i32(IP_INFO_t *ip_info, uint32_t src_ip, uint32_t dst_ip) {
//    memset(&ip_info_, 0, sizeof(IP_INFO_t));
    uint8_t index;
    for ( index = 0; index < 4; ++index) {
        ip_info->src_ip[index] = src_ip >> (12-4*index) & 0xFF;
        ip_info->dst_ip[index] = dst_ip >> (12-4*index) & 0xFF;
    }
    return 0;
}

int32_t init_src_port(UDP_INFO_t *udp_info, uint16_t src_port) {
    if(udp_info == nullptr){
        return  -1;
    }
    udp_info->src_port[0] = (src_port >> 8)&0xFF;
    udp_info->src_port[1] = (src_port)&0xFF;
    return 0;
}

int32_t init_dst_port(UDP_INFO_t *udp_info, uint16_t dst_port) {
    if(udp_info == nullptr){
        return  -1;
    }
    udp_info->dst_port[0] = (dst_port >> 8)&0xFF;
    udp_info->dst_port[1] = (dst_port)&0xFF;
    return 0;
}

int16_t init_port(UDP_INFO_t * udp_info, uint16_t src_port, uint16_t dst_port) {
    init_dst_port(udp_info, dst_port);
    init_src_port(udp_info, src_port);
    return 0;
}

int32_t init_mac_type(MAC_INFO_t *mac_info, uint16_t type) {
    //  TODO    if use others
    if(mac_info == nullptr){
        return MAC_ERROR - 2;
    }
    mac_info->ip_version[0] = (type & 0xFF00) >> 8;
    mac_info->ip_version[1] = type & 0xFF ;
    /*
    mac_info->ip_version[0] = 0x08;
    mac_info->ip_version[1] = 0x00;
    */
    return 0;
}

int32_t init_ip_headinfo(IP_INFO_t *ip_info, uint8_t version_length) {
    //  TODO if use others
    if(ip_info == nullptr){
        return IP_ERROR -1;
    }
    ip_info->IP_version_len = version_length;

    return 0;
}

int32_t init_ip_dsf(IP_INFO_t *ip_info, uint8_t DSF) {
    if(ip_info == nullptr){
        return IP_ERROR -2;
    }
    ip_info->DSF = DSF;
    return 0;
}

int32_t only_ide(IP_INFO_t *ip_info, uint16_t ide) {
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

int32_t init_flag_offset(IP_INFO_t *ip_info, uint16_t param) {
    //  TODO if set others
    if(ip_info == nullptr){
        return IP_ERROR -6;
    }
    ip_info->flag_offset[0] = (param&0xff00) >> 8;
    ip_info->flag_offset[1] = (param&0xff);
    return 0;
}

int32_t init_ttl(IP_INFO_t *ip_info, uint8_t TTL) {
    if(ip_info == nullptr){
        return IP_ERROR - 7;
    }
    ip_info->TTL_protocol[0] = TTL;
    return 0;
}

int32_t init_protocol(IP_INFO_t *ip_info, uint8_t PTL) {
    if (ip_info == nullptr){
        return IP_ERROR - 8;
    }
    ip_info->TTL_protocol[1] = PTL;
    return 0;
}

int32_t set_ip_length(IP_INFO_t *ip_info, uint16_t len) {
    ip_info->total_length[0] = (len >> 8)&0xFF;
    ip_info->total_length[1] = (len     )&0xFF;
    return 0;
}

//int dsp_sendto(const void *data, uint16_t len) {
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

int32_t set_udp_length(UDP_INFO_t *udp_info, uint16_t len) {
    udp_info->udp_length[0] = (len >> 8)&0xFF;
    udp_info->udp_length[1] = (len     )&0xFF;
    return 0;
}

int32_t dsp_add_data(UDP_PACKET_t* udp_pack, const void *data, uint16_t len) {
//    uint8_t *udp_data = (uint8_t *)malloc(len*sizeof (uint8_t));
    //  TODO
    if(udp_pack->data != nullptr){
        free(udp_pack->data);
        udp_pack->data = nullptr;
    }
    if(data == nullptr){
        return UDP_ERROR;
    }
    udp_pack->data = (uint8_t *)malloc(len*sizeof (uint8_t));
    memcpy(udp_pack->data, data, len);
    uint16_t udp_len = len+8;   //  udp head length
    set_udp_length(&udp_pack->udp_info,udp_len);
    udp_len += 20;              //  total length
    set_ip_length(&udp_pack->ip_info,udp_len);
    //  ??????ide
    uint16_t ide = get_only_ide(ide_cnt_);
    only_ide(&udp_pack->ip_info, ide);

    //  TODO calc checksum
    uint16_t ip_sum = calc_ip_checksum(udp_pack);
    printf("ip_sum: %d \r\n", ip_sum);
    set_ip_checksum(udp_pack);
    uint16_t udp_sum = calc_udp_checksum(udp_pack,udp_len-28);
    printf("udp_sum: %d \r\n", udp_sum);
    set_udp_checksum(udp_pack);
    return 0;
}


int32_t set_ip_checksum(UDP_PACKET_t* udp_pack) {
    if(udp_pack == nullptr){
        return IP_ERROR - 9;
    }
    //  clear value
    udp_pack->ip_info.ip_checksum[0] = 0x00;
    udp_pack->ip_info.ip_checksum[1] = 0x00;

    uint16_t ip_checksum = calc_ip_checksum(udp_pack);
    udp_pack->ip_info.ip_checksum[0] = (ip_checksum >> 8)&0xff;
    udp_pack->ip_info.ip_checksum[1] = (ip_checksum     )&0xff;
    return 0;
}

int32_t set_udp_checksum(UDP_PACKET_t *udp_pack) {
    if(udp_pack == nullptr){
        return UDP_ERROR - 4;
    }
    //  clear value
    udp_pack->udp_info.udp_checksum[0] = 0x00;
    udp_pack->udp_info.udp_checksum[1] = 0x00;

    uint16_t len = (udp_pack->udp_info.udp_length[0]<<8) |
                   udp_pack->udp_info.udp_length[1];
    uint16_t udp_checksum = calc_udp_checksum(udp_pack, len);
    udp_pack->udp_info.udp_checksum[0] = (udp_checksum >> 8)&0xff;
    udp_pack->udp_info.udp_checksum[1] = (udp_checksum     )&0xff;

    return 0;
}

int16_t get_pack_len(UDP_PACKET_t *udp_pack) {
    uint16_t pack_length = (udp_pack->ip_info.total_length[0]<< 8) |
                           (udp_pack->ip_info.total_length[1]);
    pack_length += 14;
    return pack_length;
}

int16_t dsp_make_package(UDP_PACKET_t *udp_pack, uint8_t **udp_package) {
    if(*udp_package != nullptr){
        return -1;
    }
//    int k_udp_head_length = 42;

    int pack_length = get_pack_len(udp_pack);
    *udp_package = (uint8_t*)malloc( pack_length*sizeof (uint8_t));
    memset(*udp_package,0,pack_length*sizeof (uint8_t));
    int k_udp_head_length = 42;
    memcpy(*udp_package, udp_pack, k_udp_head_length);
    //  test
    /*
    uint8_t strstr[12] = {};
    memcpy(strstr, udp_pack->data,pack_length-42);
    */
    memcpy((*udp_package)+42,udp_pack->data,pack_length-42);
    //  TODO send data
    printf("\r\n");
    int i = 0;
    for(i = 0; i<pack_length ; ++i){
        if(  (i % 16 == 0) && (i!=0) ){
            printf("\r\n");
        }
        printf("0x%2x ",(*udp_package)[i]);

    }
    //  free
    /*
    free(*udp_package);
    *udp_package = nullptr;
    */
    return pack_length;
}

uint16_t get_only_ide(uint16_t ide) {
    ide += 1;
    return ide;
}

int32_t init_src_ip_port(UDP_PACKET_t *udp_packet, uint8_t *src_ip, uint16_t port) {
    uint8_t ret = 0;

    //  ???????????????IP???
    init_mac_type(&udp_packet->mac_info, 0x0800);
    init_ip_headinfo(&udp_packet->ip_info, 0x45);
    init_ip_dsf(&udp_packet->ip_info, 0x00);
    //  total_len ide ??????????????????????????????
//    set_ip_length(&udp_packet->ip_info, 0x0000);
//    only_ide(&udp_packet->ip_info, 0x0000);
    init_flag_offset(&udp_packet->ip_info, 0x0000);
    init_ttl(&udp_packet->ip_info, 0x80);
    init_protocol(&udp_packet->ip_info, 0x11);
//    set_ip_checksum(udp_packet);
    ret = init_ip_addr(&udp_packet->ip_info, src_ip, nullptr);
    ret = init_src_port(&udp_packet->udp_info, port);
    return ret;
}

int32_t dsp_sendto(uint8_t eMAC_Port, UDP_PACKET_t* udp_pack, uint8_t *dst_ip, uint16_t port,
                   const void *data, uint16_t len) {
    init_ip_addr(&udp_pack->ip_info, nullptr, dst_ip);
    init_dst_port(&udp_pack->udp_info,port);
    dsp_add_data(udp_pack,data,len);
    uint8_t *complete_pack = nullptr;
    int16_t pack_len = dsp_make_package(udp_pack, &complete_pack);
//      TODO send add send packet
//    if(SendPacket(eMAC_Port, complete_pack, pack_len* sizeof(uint8_t)) != 0) {
//        return -1;
//    }
    //	free
    if(complete_pack!=nullptr){
        free(complete_pack);
        complete_pack = nullptr;
    }
    return 0;
}

int32_t dsp_recvfrom(uint8_t *recv_data, Dsp_SockAddr_t *dst, uint8_t *recv_flag) {
    //  TODO recv data
    if(*recv_flag == 0){
        return -1;
    }

//    uint8_t test_data[51] = {0x2c,0xf0,0x5d,0x16,0xff,0x57,0x2c,0x16, 0xdb,0xa1,0x94,0xf1,0x08,0x00,0x45,0x00,
//                        0x00,0x25,0xd4,0xc6,0x00,0x00,0x80,0x11, 0xe1,0xdc,0xc0,0xa8,0x01,0x6f,0xc0,0xa8,
//                        0x01,0x65,0xd4,0x31,0x30,0x39,0x00,0x11, 0x7c,0x73,0x31,0x32,0x33,0x31,0x32,0x33,
//                        0x31,0x32,0x33};

    uint8_t *dspe_recv = dsp_recv_data;
    //  =====================================

    UDP_PACKET_t udp_pack ;
    memset(&udp_pack, 0, sizeof(UDP_PACKET_t));
    int16_t data_len = dsp_unpack_package(&udp_pack, dspe_recv);
    memcpy(recv_data,udp_pack.data,data_len);
    if(udp_pack.data != nullptr){
        free(udp_pack.data);
        udp_pack.data = nullptr;
    }
    memcpy(dst->Recv_Ip,udp_pack.ip_info.src_ip,4*sizeof (uint8_t));
    memcpy(&dst->Recv_port,&udp_pack.udp_info.src_port,2*sizeof (uint8_t));
    *recv_flag = 1;
    return 0;
}

int16_t dsp_unpack_package(UDP_PACKET_t *udp_pack,const uint8_t *recv_data) {
    if(udp_pack == nullptr){
        return UDP_ERROR - 5;
    }
    if(udp_pack->data != nullptr){
        free(udp_pack->data);
    }
    int16_t pack_length = 42;
    memcpy(udp_pack, recv_data, pack_length*sizeof (uint8_t));
    int16_t data_len = (udp_pack->udp_info.udp_length[0]<<8 |
                        udp_pack->udp_info.udp_length[1]) - 8 ;
    //  !!! malloc  ????????????    !!!
    udp_pack->data = (uint8_t*) malloc(data_len* sizeof (uint8_t));
    memset(udp_pack->data,0,data_len * sizeof (uint8_t));
    memcpy(udp_pack->data,&recv_data[42],data_len);

    //  ??????????????????
    return data_len;
}


