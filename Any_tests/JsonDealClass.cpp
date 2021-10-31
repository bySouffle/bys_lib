//
// Created by bys on 2021/10/19.
//

#include "JsonDealClass.h"


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

int JsonDealClass::from_db_gen_map_json(const std::string &mapname, std::string &json) {
    rapidjson::Document d;

    return 0;
}

int JsonDealClass::from_db_gen_pos_json(const std::vector<int> &nid_list, std::string &json) {
    return 0;
}

int JsonDealClass::save_map_2_dbs(std::string &json) {

    std::string map_name;
    std::vector<int> nid_list;

    rapidjson::Document d;
    d.Parse(json.data());
    if(!d.IsObject()){
        return -1;
    }

    if(d["roadmap"].IsString()){
        map_name = d["roadmap"].GetString();
    }


    if(d["nid"].IsArray()){

    }

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