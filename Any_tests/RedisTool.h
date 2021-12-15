//
// Created by bys on 2021/9/24.
//

#ifndef TEST_REDISTOOL_H
#define TEST_REDISTOOL_H

#define TEST

#ifdef TEST
#define print(format,...) printf("FILE: " __FILE__ ", LINE %d " format "\n" , __LINE__ , ##__VA_ARGS__ )

#else
#define print(format,...)
#endif

#include <iostream>
#include <hiredis/hiredis.h>
#include <string>
#include <vector>
#include <string.h>

enum RedisStatus{
    RedisBaseStatus        = -500,
    kRedisInitERR          = RedisBaseStatus - 1,
    kRedisERR              = RedisBaseStatus - 2,
    kRediszADDERR          = RedisBaseStatus - 3,
    kRediszDELERR          = RedisBaseStatus - 4,
    kRedisitemEmpty        = RedisBaseStatus - 5,

    };

enum Data_type{
    kMove = 1,
    kPTZ,
    KLight,
    kInfrared,
    };

class RedisTool {
public:
        RedisTool();
    RedisTool(std::string &ip, int port);
    ~RedisTool();
    int init_redis();
    //  sorted set operation

    inline int zADD_item(std::string& key, Data_type type, std::string& value);
    std::vector<std::string> get_type_items(std::string& key, Data_type type);
    std::vector<std::string> get_all_items(std::string& key);

    int zADD_items(std::string& key, Data_type type, std::vector<std::string>& items, int items_size);

    int del_key(std::string& key);
    int del_type_items(std::string& key, Data_type type);

    int get_key_item_size(std::string& key);
    int get_size(std::string& key, Data_type type);

private:
    std::string redis_ip_;
    int redis_port_;

    //  链接成功 redis 生成的 fd
    redisContext* c_redis_;
};

long get_now_time_dec();

#endif //TEST_REDISTOOL_H