//
// Created by bysou on 2021/12/15.
//

#ifndef BYS_LIB_REDISSTRINGTOOL_H
#define BYS_LIB_REDISSTRINGTOOL_H

#include "RedisBaseTool.h"

class RedisStringTool :public RedisBaseTool
{
public:
    RedisStringTool(const std::string &ip, int port);
    int redis_add_a_item(const std::string &key, int nid, int item_num, const std::string &value) override;
    int redis_add_some_item (const std::string& key, int nid, std::vector<std::string>& items, int items_size) override;

    std::string redis_scan_a_item    (const std::string& key, int nid, int item_num) override;
    std::vector<std::string> redis_scan_some_item (const std::string& key, int nid) override;
};


#endif //BYS_LIB_REDISSTRINGTOOL_H
