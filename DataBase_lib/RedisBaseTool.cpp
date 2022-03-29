//
// Created by bysou on 2021/12/15.
//

#include "RedisBaseTool.h"

#include <utility>

RedisBaseTool::RedisBaseTool(std::string ip, int port)
    : redis_ip_(std::move(ip)), redis_port_(port), c_redis_(nullptr) {

}

RedisBaseTool::~RedisBaseTool() {
  if (c_redis_ != nullptr) {
    redisFree(c_redis_);
    c_redis_ = nullptr;
  }
}

//! 初始化 redis 连接
//! \return
int RedisBaseTool::init_redis() {
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

int
RedisBaseTool::redis_add_some_item(const std::string &key, int nid, std::vector<std::string> &items, int items_size) {
  return 0;
}

int RedisBaseTool::redis_add_a_item(const std::string &key, int nid, int item_num, const std::string &value) {
  return 0;
}

std::string RedisBaseTool::redis_scan_a_item(const std::string &key, int nid, int item_num) {
  return {};
}

std::vector<std::string> RedisBaseTool::redis_scan_all_item(const std::string &key) {
  return {};
}

std::vector<std::string> RedisBaseTool::redis_scan_some_item(const std::string &key, int nid) {
  return {};
}

int RedisBaseTool::redis_del_a_item(const std::string &key, int nid, int item_num) {
  return 0;
}

int RedisBaseTool::redis_del_some_item(const std::string &key, int nid, std::initializer_list<unsigned int> pos) {
  return 0;
}

int RedisBaseTool::get_nid_items_cnt(const std::string &key, int nid) {
  return 0;
}

int RedisBaseTool::get_all_items_cnt(const std::string &key) {
  return 0;
}




