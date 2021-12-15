//
// Created by bysou on 2021/12/15.
//

#include "RedisStringTool.h"

RedisStringTool::RedisStringTool(const std::string &ip, int port) : RedisBaseTool(ip, port)
{

}

int RedisStringTool::redis_add_a_item(const std::string &key, int nid, int item_num, const std::string &value) {
    if (c_redis_ == nullptr || c_redis_->err) {
        print("redis opt error: %s\r\n", c_redis_->errstr);
        return kRedisERR;
    }
    int ret_val = 0;
    redisReply *reply;
    reply = (redisReply *) redisCommand(c_redis_, "setnx %s:nid%d:%d %s", key.c_str(), nid, item_num, value.c_str());
    if (reply == nullptr) {
        print("set error : %s\r\n", reply->str);
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

std::string RedisStringTool::redis_scan_a_item(const std::string &key, int nid, int item_num) {

    if (c_redis_ == nullptr || c_redis_->err) {
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
    } else if (reply->str == nullptr) {
        print("redis get empty\r\n");
        freeReplyObject(reply);
        return {};
    } else {
        value.append(reply->str);
        freeReplyObject(reply);
        return value;
    }

}
