//
// Created by bysou on 2021/12/15.
//

#include "RedisStringTool.h"

RedisStringTool::RedisStringTool(const std::string &ip, int port) : RedisBaseTool(ip, port)
{
    init_redis();
}

int
RedisStringTool::redis_add_a_item(const std::string &key, int nid, int item_num, const std::string &value) {
    if (c_redis_ == nullptr || c_redis_->err) {
        if (c_redis_) {
            printf("Connection error: %s\n", c_redis_->errstr);
            redisFree(c_redis_);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }
    int ret_val = 0;
    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "setnx %s:nid%d:%d %s", key.c_str(), nid, item_num, value.c_str());
    if (reply == nullptr) {
        print("Connection error : %s\r\n", reply->str);
        redisFree(c_redis_);
        c_redis_ = nullptr;
        return kRedisERR;
    } else if (reply->integer == 1) {
        print("set success : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else if (reply->integer == 0) {
        print("set fail : %lld\r\n", reply->integer);
        ret_val = (int) reply->integer;
    } else {
        print("set error : %lld\r\n", reply->integer);
        ret_val = kRedisSetERR;
    }
    freeReplyObject(reply);
    return ret_val;
}

std::string
RedisStringTool::redis_scan_a_item(const std::string &key, int nid, int item_num) {
    if (c_redis_ == nullptr || c_redis_->err) {
        if (c_redis_) {
            printf("Connection error: %s\n", c_redis_->errstr);
            redisFree(c_redis_);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return {};
    }

    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "get %s:nid%d:%d", key.c_str(), nid, item_num);
    //  TODO
    std::string value{};
    if (reply == nullptr) {
        print("set error : %s\r\n", reply->str);
        redisFree(c_redis_);
        c_redis_ = nullptr;
        return {};
    }
    if (reply->str == nullptr) {
        print("redis get empty\r\n");
        freeReplyObject(reply);
        return {};
    } else {
        value.append(reply->str);
        freeReplyObject(reply);
        return value;
    }

}

int
RedisStringTool::redis_add_some_item(const std::string &key, int nid, std::vector<std::string> &items, int items_size) {
    int cnt = 0;
    for (auto &it:items) {
        int ret_val = redis_add_a_item(key, nid, cnt, it);
        if(ret_val == 1){
            cnt ++;
        }
    }
    return cnt;
}

std::vector<std::string>
RedisStringTool::redis_scan_some_item(const std::string &key, int nid) {
    if (c_redis_ == nullptr || c_redis_->err) {
        if (c_redis_) {
            printf("Connection error: %s\n", c_redis_->errstr);
            redisFree(c_redis_);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return {};
    }

    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "scan 0 match %s:nid%d:*", key.c_str(), nid);
    //  TODO
    std::string value{};
    if (reply == nullptr) {
        print("set error : %s\r\n", reply->str);
        redisFree(c_redis_);
        c_redis_ = nullptr;
        return {};
    }
    if(reply->type != REDIS_REPLY_ARRAY){
        //  scan not arr
        return {};
    }

    for (uint32_t i = 0; i < reply->element[1]->elements; ++i) {
        std::string key = reply->element[1]->element[i]->str;
        std::cout << key << std::endl;
    }


    return {};
}
