//
// Created by bys on 2021/9/24.
//

#include "RedisTool.h"

RedisTool::RedisTool() : redis_ip_("127.0.0.1"), redis_port_(6379), c_redis_(nullptr) {
    init_redis();
}


RedisTool::RedisTool(std::string &ip, int port) : redis_ip_(ip), redis_port_(port) {

}

//! 初始化 redis 连接
//! \return
int RedisTool::init_redis() {
    //  free
    if (c_redis_ != nullptr) {
        redisFree(c_redis_);
        c_redis_ = nullptr;
    }
    struct timeval timeout = {1, 500000};
    c_redis_ = redisConnectWithTimeout(redis_ip_.data(), redis_port_, timeout);
    if (c_redis_->err) {
        print("init error : %s\r\n", c_redis_->errstr);
        return kRedisInitERR;
    }
    return 0;
}

RedisTool::~RedisTool() {
    if (c_redis_ != nullptr) {
        redisFree(c_redis_);
        c_redis_ = nullptr;
    }
}

//! 补充一条元素到有序集合
//! \param key
//! \param type 集合的 score value
//! \param value
//! \return
int RedisTool::zADD_item(std::string &key, unsigned int type, std::string &value) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }
    int ret_val = 0;
    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "zADD %s %d %s", key.c_str(), type, value.c_str());
    if (reply == nullptr) {
        print("zADD error : %s\r\n", reply->str);
        redisFree(c_redis_);
        c_redis_ = nullptr;
        return kRedisERR;
    } else if (reply->integer == 1) {
        print("zADD success : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else if (reply->integer == 0) {
        print("zADD success : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else {
        print("zADD error : %lld\r\n", reply->integer);
        ret_val = kRediszADDERR;
    }
    freeReplyObject(reply);
    return ret_val;
}


//! 返回对应 score 的所有元素
//! \param key 集合的 score value
//! \param type
//! \return vector
std::vector<std::string> RedisTool::get_type_items(std::string &key, unsigned int type) {

    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return std::vector<std::string>();
    }

    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "ZCOUNT %s %d %d", key.c_str(), type, type);

    int valueSize = (int) reply->integer;
    print("item size: %d\r\n", valueSize);
    //    reply = (redisReply *) redisCommand(c_redis_, "ZRANGEBYSCORE %s (%d %d", key.c_str(), 0, valueSize - 1);
    reply = (redisReply *) redisCommand(c_redis_, "ZRANGEBYSCORE %s %d %d", key.c_str(), type, type);

    print("item type: %d\r\n", reply->type);
    print("item elements size: %ld\r\n", reply->elements);

    redisReply **replyVector = reply->element;//获取数组指针
    std::vector<std::string> result;
    for (int i = 0; i < valueSize; i++) {
        result.emplace_back((*replyVector)->str);
        print("value: %s\r\n", (*replyVector)->str);
        replyVector++;
    }

    return result;

}

//! 补充一组 item
//! \param key
//! \param type
//! \param items
//! \param items_size
//! \return 返回添加的成功的条目
int RedisTool::zADD_items(std::string &key, unsigned int type, std::vector<std::string> &items, int items_size) {
    int cnt = 0;
    for (int i = 0; i < items_size; ++i) {
        int ret_val = zADD_item(key, type, items.at(i));
        if( ret_val >= 0 ){
            cnt ++;
        }
    }
    return cnt;
}

//! 删除所有 key
//! \param key
//! \return
int RedisTool::del_key(std::string &key) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }
    int ret_val = 0;
    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "DEL %s", key.c_str());
    if (reply == nullptr) {
        print("del error : %s\r\n", reply->str);
        redisFree(c_redis_);
        c_redis_ = nullptr;
        return kRedisERR;
    } else if (reply->integer == 1) {
        print("del success : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else if (reply->integer == 0) {
        print("empty keys : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else {
        print("del error : %lld\r\n", reply->integer);
        ret_val = kRediszDELERR;
    }
    freeReplyObject(reply);
    return 0;
}

//! 删除对应 type 的所有key
//! \param key
//! \param type
//! \return
int RedisTool::del_type_items(std::string &key, unsigned int type) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }
    int ret_val = 0;
    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "ZREMRANGEBYSCORE %s %d %d", key.c_str(), type, type);
    if (reply == nullptr) {
        print("del error : %s\r\n", reply->str);
        redisFree(c_redis_);
        c_redis_ = nullptr;
        return kRedisERR;
    } else if (reply->integer > 0) {
        print("del success : %lld items\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else if (reply->integer == 0) {
        print("empty keys : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else {
        print("del error : %lld\r\n", reply->integer);
        ret_val = kRediszDELERR;
    }
    freeReplyObject(reply);
    return reply->integer;
}

std::vector<std::string> RedisTool::get_all_items(std::string &key) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return {};
    }

    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "ZCOUNT %s ", key.c_str());
    //
    //    int valueSize = (int) reply->integer;
    //    print("item size: %d\r\n", valueSize);
    // ZRANGEBYSCORE salary -inf +inf
    reply = (redisReply *) redisCommand(c_redis_, "ZRANGE %s 0 -1", key.c_str());
    print("item type: %d\r\n", reply->type);
    print("item elements size: %ld\r\n", reply->elements);

    redisReply **replyVector = reply->element;//获取数组指针
    if(replyVector == nullptr){
        print("No items\r\n");
        return {};
    }
    std::vector<std::string> result;
    for (int i = 0; i < reply->elements; i++) {
        result.emplace_back((*replyVector)->str);
        print("value: %s\r\n", (*replyVector)->str);
        replyVector++;
    }

    return result;
}

int RedisTool::get_size(std::string &key, unsigned int type) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }

    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "ZCOUNT %s %d %d", key.c_str(), type, type);

    //    int valueSize = (int) reply->integer;
    return (int) reply->integer;
}

int RedisTool::get_key_item_size(std::string &key) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }

    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "ZCARD %s", key.c_str() );

    //    int valueSize = (int) reply->integer;
    return (int) reply->integer;
}

std::multimap<int, std::string> RedisTool::get_all_scores_key(std::string &key) {

    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return {};
    }

    redisReply *reply;
//    reply = (redisReply *) redisCommand(c_redis_, "ZCARD %s ", key.c_str());
//    unsigned int valueSize = reply->integer;
//    if( valueSize == 0){
//        return {};
//    }

    reply = (redisReply *) redisCommand(c_redis_, "ZRANGE %s 0 -1 WITHSCORES", key.c_str());
    print("item type: %d\r\n", reply->type);
    print("item elements size: %ld\r\n", reply->elements);

    redisReply **replyVector = reply->element;//获取数组指针
    if(replyVector == nullptr){
        print("No items\r\n");
        return {};
    }
    std::multimap<int, std::string> result;
    int map_key;
    std::string map_val;

    for (int i = 0; i < reply->elements; i++) {

        if( (i%2) == 0){
            map_val.clear();
            map_val = (*replyVector)->str;
            print("value: %s\r\n", (*replyVector)->str);

        }
        if((i%2) == 1){
            map_key = strtol( (*replyVector)->str, nullptr, 10);
            result.insert(std::pair<int, std::string>(map_key, map_val));
            print("value: %s\r\n", (*replyVector)->str);
        }

        replyVector++;
    }

    return result;
}


long get_now_time_dec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm t;
    t = *localtime(&tv.tv_sec);


    int iY, iM, iD, iH, iMin, iS;

    iY = t.tm_year + 1900;
    iM = t.tm_mon + 1;
    iD = t.tm_mday;
    iH = t.tm_hour;
    iMin = t.tm_min;
    iS = t.tm_sec;


    //    print("%d-%0d-%0d %0d:%02d:%02d\n", iY, iM, iD, iH, iMin, iS);
    //    print("%ld\n",(long)mktime(&t));
    return (long)mktime(&t);}