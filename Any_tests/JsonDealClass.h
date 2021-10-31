//
// Created by bys on 2021/10/19.
//

#ifndef BYS_LIB_JSONDEALCLASS_H
#define BYS_LIB_JSONDEALCLASS_H

#include "cJSON.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <string.h>
#include <map>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


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



/*================    json format      ===================*/
/* *
 *
{
"roadmap" : "map1",
"nid" : 0,

"timestamp" : 2342315234,
"args": [
    {
        "pose" : {
        "xyz" : [1.52, 3.55, 0.0],
        "rpy" : [0.0, 0.0, 1.2],
        "blh" : null
        },
        "vel" : { "enu" : null, "heading":0.8 }
    }
  ]
}
 *
 *
 *  database key        ==>  地图   map1    map2    ...
 *           source     ==>  坐标   pos1 ==> 1    pos2 ==> 2
 *           value      ==>
                          { "function": "chs",
                            "value": {
                                  "xyz": [1.1, 2.2, 3.3],
                                  "rpy": [1.1, 2.2, 3.3],
                                  "blh": null
                                  }
                           }

                          { "function": "ptz",
                            "value": {
                                  "cmd": "ctl_level_left",
                                  "H": "1f",
                                  "L": "2b"
                                  }
                           }

                          { "function": "vlc",
                            "value": {
                                  "cmd": "get_a_picture",
                                  "action": "None",
                                  "pos1":[ [-1, -1], [-1, -1] ],
                                  "save_url": "/a/b/c",
                                  "reserve": "etc"
                                  }
                           }

                          { "function": "irc",
                            "value": {
                                  "cmd": "ptz_iris_close_stop",
                                  "action": "Identify",
                                  "pos1":[ [2.4, 2,4], [3.1, 2.4] ],
                                  "reserve": "etc"
                                  }
                           }

 *  =======================================================
 *  定义导出的json格式
{
  "roadmap": "map1",
  "nid_size": 7,
  "nid1": [
    {
      "function": "chs",
      "value": {
        "xyz": [
          1.1,
          2.2,
          3.3
        ],
        "rpy": [
          1.1,
          2.2,
          3.3
        ],
        "blh": null
      }
    },
    {
      "function": "ptz",
      "value": {
        "cmd": "ctl_level_left",
        "H": "1f",
        "L": "2b"
      }
    }
  ],
  "nid2": [
    {
      "function": "chs",
      "value": {
        "xyz": [
          1.1,
          2.2,
          3.3
        ],
        "rpy": [
          1.1,
          2.2,
          3.3
        ],
        "blh": null
      }
    },
    {
      "function": "ptz",
      "value": {
        "cmd": "ctl_level_left",
        "H": "1f",
        "L": "2b"
      }
    }
  ]
}
 *
 *
 * */
/*========================================================*/

/*========================================================*/
/*  自动模式开启时传入的参数
 *      *   { "roadmap": "map_a", "nid": [1,2,3,4] }
 *
 *
 *
 */


//template< typename... Args >
//std::string string_sprintf( const char* format, Args... args ) {
//    int length = std::snprintf( nullptr, 0, format, args... );
//    assert( length >= 0 );
//
//    char* buf = new char[length + 1];
//    std::snprintf( buf, length + 1, format, args... );
//
//    std::string str( buf );
//    delete[] buf;
//    return std::move(str);
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

template <typename T>
int replace_json_symbol_str(std::string &json, T t) {
    unsigned long index = json.find_first_of('%');
    if(index != std::string::npos){
        json.erase(index,1);
        json.insert(index, t);
    }
}

template <typename T,typename ...U>
int replace_json_symbol_str(std::string &json, T t, U ...u) {
    unsigned long index = json.find_first_of('%');
    std::cout << index <<" ";
    if(index != std::string::npos){
        json.erase(index,1);

        json.insert(index, t);
        replace_json_symbol_str(json, u...);
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

long get_now_time_dec();

class JsonDealClass {
public:
    int parse_chassis_data_to_dbs(std::string &json,
                                  std::string &map_name, int &pos_id, std::string &dbs_value) ;

    int parse_automode_param(std::string &json, std::string& map_name, std::vector<int>& nid_list);
    int parse_nid_dbs_to_subvec(std::vector<std::string> &vec_list,
                                std::vector<std::string> &vec_chs_cmd, std::vector<std::string> &vec_chs,
                                std::vector<std::string> &vec_ptz_cmd, std::vector<std::string> &vec_ptz,
                                std::vector<std::string> &vec_vlc_cmd, std::vector<std::string> &vec_vlc,
                                std::vector<std::string> &vec_irc_cmd, std::vector<std::string> &vec_irc);

    int from_db_gen_map_json(const std::string& mapname, std::string& json);
    int from_db_gen_pos_json(const std::vector<int >& nid_list, std::string& json);
    int save_map_2_dbs(std::string& json);
private:

};



#endif //BYS_LIB_JSONDEALCLASS_H