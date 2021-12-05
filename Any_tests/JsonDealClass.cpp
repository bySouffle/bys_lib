//
// Created by bys on 2021/10/19.
//

#include "JsonDealClass.h"

RedisTool r();

int JsonDealClass::parse_chassis_data_to_dbs(std::string &json, std::string &map_name, int &pos_id,
                                             std::string &dbs_value) {
    cJSON *root = cJSON_Parse(json.data());
    if (root == nullptr) return kRootParseErr;

#if 0   //  解析dbs数据
    std::vector<double> xyz_value;
    std::vector<double> rpy_value;
    int blh_value = -1;

    cJSON *map_name_item = cJSON_GetObjectItem(root, "map_name");
    if(map_name_item == nullptr)    return kSubParseErr;
    map_name.append(cJSON_GetStringValue(map_name_item));

    cJSON *dbs_value_item = cJSON_GetObjectItem(root, "pose");
    if(dbs_value_item == nullptr)    return kSubParseErr;
    cJSON * xyz = cJSON_GetObjectItem(dbs_value_item, "xyz");
    if(cJSON_IsArray(xyz)){
        int arr_len = cJSON_GetArraySize(xyz);
        for (int i = 0; i < arr_len; ++i) {
            cJSON *xyz_value_item = cJSON_GetArrayItem(xyz,i);
            if (xyz_value_item == nullptr){
                continue;
            }
            xyz_value.emplace_back(xyz_value_item->valuedouble);
        }
    }

    cJSON * rpy = cJSON_GetObjectItem(dbs_value_item, "rpy");
    if(cJSON_IsArray(rpy)){
        int arr_len = cJSON_GetArraySize(rpy);
        for (int i = 0; i < arr_len; ++i) {
            cJSON *rpy_value_item = cJSON_GetArrayItem(rpy,i);
            if (rpy_value_item == nullptr){
                continue;
            }
            rpy_value.emplace_back(rpy_value_item->valuedouble);
        }
    }

    cJSON * blh = cJSON_GetObjectItem(dbs_value_item, "blh");
    if(blh->type == cJSON_NULL){
        blh_value = 0;
    }
#endif
    cJSON *map_items = cJSON_GetObjectItem(root, "roadmap");
    map_name.clear();
    map_name.append(cJSON_GetStringValue(map_items));

    cJSON *pos_items = cJSON_GetObjectItem(root, "nid");
    pos_id = pos_items->valueint;
    dbs_value.clear();
    dbs_value.append(R"({ "function": "chs", "value": %})");
    //  解析底盘发来的数据
    cJSON *args_items = cJSON_GetObjectItem(root, "args");
    if ((args_items != nullptr) && cJSON_IsArray(args_items)) {
        int arr_size = cJSON_GetArraySize(args_items);
        cJSON *xyz_arr = cJSON_GetArrayItem(args_items, 0);
        cJSON *xyz_item = cJSON_GetObjectItem(xyz_arr, "pose");
        int index = dbs_value.find_first_of('%');
        dbs_value.erase(index, 1);
        dbs_value.insert(index, cJSON_Print(xyz_item));
    }
    return 0;
}

int JsonDealClass::parse_automode_param(std::string &json, std::string &map_name, std::vector<int> &vec_nid) {
    cJSON *root = cJSON_Parse(json.data());
    if (root == nullptr) return kRootParseErr;

    cJSON *road_map = cJSON_GetObjectItem(root, "roadmap");
    if (road_map == nullptr) return kSubParseErr;
    map_name.append(cJSON_GetStringValue(road_map));

    cJSON *nid_item = cJSON_GetObjectItem(root, "nid");
    if ((nid_item != nullptr) && cJSON_IsArray(nid_item)) {
        int arr_size = cJSON_GetArraySize(nid_item);
        for (int i = 0; i < arr_size; ++i) {
            vec_nid.emplace_back(cJSON_GetArrayItem(nid_item, i)->valueint);
        }
    } else {
        return kSubParseErr;
    }

    return 0;
}

int JsonDealClass::parse_nid_dbs_to_subvec(std::vector<std::string> &vec_list,
                                           std::vector<std::string> &vec_chs_cmd, std::vector<std::string> &vec_chs,
                                           std::vector<std::string> &vec_ptz_cmd, std::vector<std::string> &vec_ptz,
                                           std::vector<std::string> &vec_vlc_cmd, std::vector<std::string> &vec_vlc,
                                           std::vector<std::string> &vec_irc_cmd, std::vector<std::string> &vec_irc) {
    for (auto &i : vec_list) {

        cJSON *root = cJSON_Parse(i.data());
        if (root != nullptr) {
            cJSON *func_item = cJSON_GetObjectItem(root, "function");
            if (func_item != nullptr) {
                std::string func_value(cJSON_GetStringValue(func_item));
                if (func_value == "chs") {
                    std::string str_payload(R"( {
                            "timestamp": %,
                            "args": [
                            {
                                "roadmap": "map_a",
                                "nid": [],
                                "coord": [[%, %]]
                    }]} )");

                    double x = 0;
                    double y = 0;
                    {
                        cJSON *val_item = cJSON_GetObjectItem(root, "value");
                        if (val_item != nullptr) {
                            cJSON *xyz_item = cJSON_GetObjectItem(val_item, "xyz");
                            if ((xyz_item != nullptr) && cJSON_IsArray(xyz_item)) {
                                x = cJSON_GetArrayItem(xyz_item, 0)->valuedouble;
                                y = cJSON_GetArrayItem(xyz_item, 1)->valuedouble;
                            }
                        }
                        long timestamp = get_now_time_dec();
                        replace_json_symbol(str_payload, timestamp, x, y);
                        //                        vec_vlc_cmd.emplace_back();
                        vec_chs.emplace_back(str_payload);
                    }
                } else if (func_value == "ptz") {
                    std::string str_payload(R"({"cmd": "%", "H": "%", "L": "%"})");

                    std::string cmd;
                    std::string H;
                    std::string L;

                    {
                        cJSON *val_item = cJSON_GetObjectItem(root, "value");
                        if (val_item != nullptr) {
                            cJSON *cmd_item = cJSON_GetObjectItem(val_item, "cmd");
                            if (cmd_item != nullptr) cmd.append(cJSON_GetStringValue(cmd_item));

                            cJSON *H_item = cJSON_GetObjectItem(val_item, "H");
                            if (H_item != nullptr) H.append(cJSON_GetStringValue(H_item));

                            cJSON *L_item = cJSON_GetObjectItem(val_item, "L");
                            if (L_item != nullptr) L.append(cJSON_GetStringValue(L_item));
                        }
                        replace_json_symbol_str(str_payload, cmd, H, L);
                        vec_ptz_cmd.emplace_back(cmd);
                        vec_ptz.emplace_back(str_payload);
                    }
                } else if (func_value == "vlc") {
                    std::string str_payload(R"({"cmd": "%", "reserve": "%"})");

                    std::string cmd;
                    std::string Reserve;

                    {
                        cJSON *val_item = cJSON_GetObjectItem(root, "value");
                        if (val_item != nullptr) {
                            cJSON *cmd_item = cJSON_GetObjectItem(val_item, "cmd");
                            if (cmd_item != nullptr) cmd.append(cJSON_GetStringValue(cmd_item));

                            cJSON *Res_item = cJSON_GetObjectItem(val_item, "reserve");
                            if (Res_item != nullptr) Reserve.append(cJSON_GetStringValue(Res_item));

                        }
                        replace_json_symbol_str(str_payload, cmd, Reserve);
                        vec_vlc_cmd.emplace_back(cmd);
                        vec_vlc.emplace_back(str_payload);

                    }
                } else if (func_value == "irc") {
                    std::string str_payload(R"({"cmd": "%", "reserve": "%"})");

                    std::string cmd;
                    std::string Reserve;

                    {
                        cJSON *val_item = cJSON_GetObjectItem(root, "value");
                        if (val_item != nullptr) {
                            cJSON *cmd_item = cJSON_GetObjectItem(val_item, "cmd");
                            if (cmd_item != nullptr) cmd.append(cJSON_GetStringValue(cmd_item));

                            cJSON *Res_item = cJSON_GetObjectItem(val_item, "reserve");
                            if (Res_item != nullptr) Reserve.append(cJSON_GetStringValue(Res_item));
                        }
                        replace_json_symbol_str(str_payload, cmd, Reserve);
                        vec_irc_cmd.emplace_back(cmd);
                        vec_irc.emplace_back(str_payload);

                    }
                } else {


                }


            }
        }


    }

    return 0;
}

int JsonDealClass::from_db_gen_map_json(const std::string &cmd_json, std::string &ret_json) {
    //  公共参数
    std::string map_name;
//    std::vector<int> nid_list;
    int nid_size = 0;
    int nid_list[64] = {};
    //  解析命令
    //      获取地图名
    cJSON *cmd_root = cJSON_Parse(cmd_json.data());
    if(cmd_root == nullptr){
        return -1;
    }
    cJSON *key_cmd_map_name = cJSON_GetObjectItem(cmd_root, "mapname");
    if(key_cmd_map_name!= nullptr){
        map_name.append(cJSON_GetStringValue(key_cmd_map_name));
    }
    //      获取地图节点
    cJSON *key_cmd_nid_list = cJSON_GetObjectItem(cmd_root, "nid");
    if(cJSON_IsArray(key_cmd_nid_list)){
        nid_size = cJSON_GetArraySize(key_cmd_nid_list);
        for (int i = 0; i < nid_size; ++i) {
            nid_list[i] = (cJSON_GetArrayItem(key_cmd_nid_list,i)->valueint);
        }
    }
    cJSON_Delete(cmd_root);

    //  输出json
    cJSON *root = cJSON_CreateObject();
    if(root == nullptr){
        return -1;
    }
    //      组成基本信息
    cJSON_AddStringToObject(root,"roadmap", map_name.data());
    cJSON_AddNumberToObject(root, "nid_size", nid_size);
    cJSON * nid_list_arr = cJSON_CreateIntArray(nid_list, nid_size);
    cJSON_AddItemToObject(root, "nid", nid_list_arr);

    for (int i = 0; i < nid_size; ++i) {
        std::string key_nid_n("nid");
        key_nid_n.append(std::to_string(nid_list[i]));

        cJSON *nid_info = cJSON_CreateArray();
        std::vector<std::string> db_nid_info = r->get_type_items(map_name,nid_list[i]);
        for (int j = 0; j < db_nid_info.size(); ++j) {
            cJSON *db_nid_item_info = cJSON_Parse(db_nid_info.at(j).data());
            cJSON_AddItemToArray(nid_info, db_nid_item_info);
        }
        cJSON_AddItemToObject(root,key_nid_n.data(),nid_info);

    }
    printf("%s\n", cJSON_Print(root)) ;
    cJSON_Delete(root);

    return 0;
}

//int JsonDealClass::from_db_gen_pos_json(const std::vector<int> &nid_list, std::string &json) {
//
//    return 0;
//}

int JsonDealClass::save_map_2_dbs(const std::string &json) {
    std::string map_name;
    std::vector<int> nid_list;

    cJSON* root = cJSON_Parse(json.data());
    if(root == nullptr){
        return -1;
    }

    //  获取地图名
    cJSON *key_roadmap = cJSON_GetObjectItem(root, "roadmap");
    if(key_roadmap == nullptr){
        return -1;
    }
    map_name.append(cJSON_GetStringValue(key_roadmap));

    //  获取位置点
    cJSON *key_nid_list = cJSON_GetObjectItem(root, "nid");
    if(cJSON_IsArray(key_nid_list)){
        int size = cJSON_GetArraySize(key_nid_list);
        for (int i = 0; i < size; ++i) {
             nid_list.emplace_back( cJSON_GetArrayItem(key_nid_list,i)->valueint );
        }
    }

    //  解析点
    for (int i = 0; i < nid_list.size(); ++i) {
        std::string now_nid("nid");
        now_nid.append(std::to_string(i));
        cJSON *key_now_nid = cJSON_GetObjectItem(root, now_nid.data());
        if(cJSON_IsArray(key_nid_list)){
            int size = cJSON_GetArraySize(key_now_nid);
            for (int j = 0; j < size; ++j) {
                cJSON *sub_step = cJSON_GetArrayItem(key_now_nid,j);
                std::string save_data(cJSON_Print(sub_step));
                r->zADD_item(map_name,nid_list.at(i), save_data);
            }
        }
    }

    return 0;
}

JsonDealClass::JsonDealClass():r(new RedisTool()) {

}

JsonDealClass::~JsonDealClass() {
    delete r;
}

int JsonDealClass::from_db_gen_map_all_json(const std::string &cmd_json, std::string &ret_json) {
    //  1. parse map_name
    std::string map_name;
    {
        cJSON *cmd_root = cJSON_Parse(cmd_json.data());
        if(cmd_root == nullptr){
            return -1;
        }
        cJSON *key_cmd_map_name = cJSON_GetObjectItem(cmd_root, "roadmap");
        if(key_cmd_map_name!= nullptr){
            map_name.append(cJSON_GetStringValue(key_cmd_map_name));
        }
        cJSON_Delete(cmd_root);
    }

    //  gen json
    {
        cJSON * root = cJSON_CreateObject();
        if(root == nullptr)     return -1;

        cJSON_AddStringToObject(root, "roadmap", map_name.data());

        cJSON_AddItemToObject(root, "timestamp", cJSON_CreateNumber(get_now_time_dec()) );

        std::string nid_key;

        std::vector<int> nid_vec;
        std::vector<std::string> nid_info_vec;

        std::multimap< int, std::string > dbs_result = r->get_all_scores_key(map_name);
        if(dbs_result.empty()){
            return -1;
        }

        /*
         *  1. point vector is empty
         *
         *
         * */
        cJSON *nid_info = nullptr;
//        for ( auto &it: dbs_result ) {
        for (auto & it : dbs_result) {
            if ( nid_vec.empty() ) {
                nid_vec.push_back( it.first );

//                cJSON *db_nid_item_info = cJSON_Parse(it->second .data());
//                cJSON_AddItemToArray(nid_info, db_nid_item_info);
                nid_info_vec.emplace_back( it.second );

            } else {
                if ( nid_vec.at( nid_vec.size() -1 ) == it.first ) {
//                    cJSON *db_nid_item_info = cJSON_Parse(it->second .data());
//                    cJSON_AddItemToArray(nid_info, db_nid_item_info);
                    nid_info_vec.emplace_back( it.second );
//                    //  最后一组处理
//                    auto it_end = it;
//                    if(( it_end++ ) == (dbs_result.end() )){
//                        std::string key_nid_n("nid");
//                        key_nid_n.append(std::to_string(nid_vec.at(nid_vec.size() -1) ));
//                        cJSON *nid_info = cJSON_CreateArray();
//                        for (int i = 0; i < nid_info_vec.size(); ++i) {
//                            cJSON *db_nid_item_info = cJSON_Parse(nid_info_vec.at(i) .data());
//                            cJSON_AddItemToArray(nid_info, db_nid_item_info);
//                        }
////                    cJSON *db_nid_item_info = cJSON_Parse(it->second .data());
////                    cJSON_AddItemToArray(nid_info, db_nid_item_info);
//
//                        cJSON_AddItemToObject(root,key_nid_n.data(),nid_info);
//
//                        std::cout << "e=============================================\n";
//                        std::cout << cJSON_Print(root);
//                        std::cout << "e=============================================\n";
//                    }
                } else {
                    //  1. 保存到cjosn arr
                    cJSON *nid_info_js_arr = cJSON_CreateArray();
                    for (auto & i : nid_info_vec) {
                        cJSON *db_nid_item_info = cJSON_Parse(i .data());
                        cJSON_AddItemToArray(nid_info_js_arr, db_nid_item_info);
                    }
                    std::string key_nid_n("nid");
                    key_nid_n.append(std::to_string(nid_vec.at(nid_vec.size() -1) ));
                    cJSON_AddItemToObject(root, key_nid_n.data(), nid_info_js_arr);
                    std::cout << "1=============================================\n";
                    std::cout << cJSON_Print(root);
                    std::cout << "1=============================================\n";
                    //  2. 重置添加新变量
                    nid_info_vec.clear();
                    nid_vec.push_back( it.first );


//                    cJSON *db_nid_item_info = cJSON_Parse(it->second .data());
//                    cJSON_AddItemToArray(nid_info_js_arr, db_nid_item_info);
                    nid_info_vec.emplace_back( it.second );
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                }
            }
        }

        //  last deal
        std::string key_nid_n("nid");
        key_nid_n.append(std::to_string(nid_vec.at(nid_vec.size() -1) ));
        cJSON *nid_info_last = cJSON_CreateArray();
        for (int i = 0; i < nid_info_vec.size(); ++i) {
            cJSON *db_nid_item_info = cJSON_Parse(nid_info_vec.at(i) .data());
            cJSON_AddItemToArray(nid_info_last, db_nid_item_info);
        }
//                    cJSON *db_nid_item_info = cJSON_Parse(it->second .data());
//                    cJSON_AddItemToArray(nid_info, db_nid_item_info);

        cJSON_AddItemToObject(root,key_nid_n.data(),nid_info_last);

        print("last data %s\n", nid_info_vec.at(0).data());
        //

        //        const int nid_size = nid_vec.size();
        unsigned int nid_size = nid_vec.size();

        int nid_num[nid_size];
        for (int i = 0; i < nid_vec.size(); ++i) {
            nid_num[i] = nid_vec.at(i);
        }

        cJSON_AddItemToObject(root, "nid", cJSON_CreateIntArray(nid_num,nid_vec.size()) );

        std::cout << "ee=============================================\n";
        std::cout << cJSON_Print(root);
        cJSON_Delete(root);
        std::cout << "ee=============================================\n";

//        cJSON *nid_info = cJSON_CreateArray();

//        for(auto &it: dbs_result){
//            if( nid_vec.empty()) {
//                nid_vec.push_back(it.first);
//                nid_key.clear();
//                nid_key.append("nid");
//                nid_key.append(std::to_string(it.first));
//            } else if(( nid_vec.at( nid_vec.size() -1 ) != it.first) ){
//                nid_vec.push_back(it.first);
//                nid_key.clear();
//                nid_key.append("nid");
//                nid_key.append(std::to_string(it.first));
//                cJSON_AddItemToObject(root, nid_key.data(), nid_info);
//            } else if( ( nid_vec.at( nid_vec.size() -1 ) == it.first) ){
//                cJSON *nid_item_info = cJSON_Parse(it.second.data());
//                cJSON_AddItemToArray(nid_info, nid_item_info);
//
//            }





    }



    return 0;
}

int JsonDealClass::arrive_parse(const std::string &cmd_json) {
    cJSON *root = cJSON_Parse(cmd_json.data());
    std::string status ;
    if(root!= nullptr){
        cJSON *arg_key = cJSON_GetObjectItem(root, "args");
        if(cJSON_IsArray(arg_key)){
            cJSON *data_item = cJSON_GetArrayItem(arg_key, 0);
            status.append( cJSON_GetStringValue(cJSON_GetObjectItem(data_item, "status")));
        }
        cJSON_Delete(root);
    }

    std::cout << status;
    return 0;
}




//std::string doubleToString(const double &dbNum) {
//
//        char *chCode;
//        chCode = new(std::nothrow)char[20];
//        sprintf(chCode, "%.2lf", dbNum);
//        std::string strCode(chCode);
//        delete []chCode;
//        return strCode;
//
//
//}