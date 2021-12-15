//
// Created by bysou on 2021/12/15.
//

#include "RedisStringTool.h"
#include "gtest/gtest.h"

TEST(redis_string_add, a){
    RedisStringTool r("127.0.0.1", 6379);
    r.init_redis();
    r.redis_add_a_item("test", 0, 0, "zzz");
    std::cout << r.redis_scan_a_item("test",0,0) <<std::endl;
}