//
// Created by bys on 2021/10/19.
//

#ifndef BYS_LIB_JSONDEALCLASS_H
#define BYS_LIB_JSONDEALCLASS_H

#include "cJSON.h"
#include <iostream>
#include <string>
#include <vector>
enum JsonExecStatus{
    kRootParseErr = -1,
    kSubParseErr  = -2,

};

//std::string doubleToString(const double &dbNum);
//
//template <typename T>
//int replace_json_symbol(std::string &json, std::initializer_list<T> args){
//
//    for(auto arg : args){
//        int index = json.find_first_of('%');
//        json.erase(index,1);
//        json.insert(index, doubleToString(arg));
//    }
//
//}

//  replace json
template <typename T>
int replace_json_symbol(std::string &json, T t) {
    unsigned long index = json.find_first_of('%');
    if(index != std::string::npos){
        json.erase(index,1);
        json.insert(index, std::to_string(t));
    }
}

template <typename T,typename ...U>
int replace_json_symbol(std::string &json, T t, U ...u) {
    unsigned long index = json.find_first_of('%');
    std::cout << index <<" ";
    if(index != std::string::npos){
        json.erase(index,1);
        json.insert(index, std::to_string(t));
        replace_json_symbol(json, u...);
    }
}

//template <typename ...U>
//int replace_json_symbol(std::string &json,  U ...u) {
//    auto f = []<typename Arg>(Arg arg, std::string& json ){
//        unsigned long index = json.find_first_of('%');
//        if(index != std::string::npos){
//            json.erase(index,1);
//            json.insert(index, std::to_string(arg));
//        }
//    };
//        (f(u,json),...);
//}

class JsonDealClass {
public:
    int parse_chassis_data(std::string &json, std::string &map_name,
                           std::string &dbs_value);

private:
};



#endif //BYS_LIB_JSONDEALCLASS_H
