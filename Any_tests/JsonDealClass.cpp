//
// Created by bys on 2021/10/19.
//

#include "JsonDealClass.h"

int JsonDealClass::parse_chassis_data(std::string &json, std::string &map_name, std::string &dbs_value) {
    cJSON* root = cJSON_Parse(json.data());
    if(root == nullptr)     return kRootParseErr;

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

    std::string exec_json(R"( { "xyz": [%, %, %], "rpy": [%, %, %], "blh": % } )");

//    replace_json_symbol(exec_json, xyz_value);


    return 0;
}

std::string doubleToString(const double &dbNum) {

        char *chCode;
        chCode = new(std::nothrow)char[20];
        sprintf(chCode, "%.2lf", dbNum);
        std::string strCode(chCode);
        delete []chCode;
        return strCode;


}
