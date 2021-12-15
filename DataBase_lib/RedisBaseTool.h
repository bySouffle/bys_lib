//
// Created by bysou on 2021/12/15.
//

#ifndef BYS_LIB_REDISBASETOOL_H
#define BYS_LIB_REDISBASETOOL_H

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

enum RedisStringStatus{
    RedisBaseStatus        = -500,
    kRedisInitERR          = RedisBaseStatus - 1,
    kRedisERR              = RedisBaseStatus - 2,
    kRedisSetERR          = RedisBaseStatus - 3,
    kRedisdelERR          = RedisBaseStatus - 4,
    kRedisitemEmpty        = RedisBaseStatus - 5,

};

class RedisBaseTool {
public:
    RedisBaseTool(std::string ip, int port );
    ~RedisBaseTool();
    
    
    //  mapname:nidi:item_i value
    //  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>   add     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    virtual int redis_add_a_item    (const std::string& key, int nid, int item_num, const std::string& value);
    virtual int redis_add_some_item (const std::string& key, int nid, std::vector<std::string>& items, int items_size);
    //  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<  add end  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>   search     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    virtual std::string redis_scan_a_item    (const std::string& key, int nid, int item_num);
    virtual std::vector<std::string> redis_scan_some_item (const std::string& key, int nid);
    virtual std::vector<std::string> redis_scan_all_item  (const std::string& key);
    //  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<  search  end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>     del      >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    virtual int redis_del_a_item    (const std::string& key, int nid, int item_num);
    virtual int redis_del_some_item (const std::string& key, int nid, std::initializer_list<unsigned int> pos);
    //  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<  del  end    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>     cnt      >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    virtual int  get_nid_items_cnt    (const std::string& key, int nid);
    virtual int  get_all_items_cnt    (const std::string& key);

    //  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<  cnt  end    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    int init_redis();
private:

    std::string redis_ip_;
    int redis_port_;

protected:
    redisContext* c_redis_;


};


#endif //BYS_LIB_REDISBASETOOL_H
