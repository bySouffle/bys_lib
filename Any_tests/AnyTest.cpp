//
// Created by bysou on 2021/9/21.
//

#include "RedisTool.h"
#include "JsonDealClass.h"
#include "gtest/gtest.h"
#include "TempClass.h"
#include "Buffer.h"

TEST(hellow, out){
    printf("hello wolrd\r\n");
}

TEST(bind, out){
    THREAD_TEST t;
    auto func = std::bind(&THREAD_TEST::func1, &t, std::placeholders::_1);
    auto func2 = std::bind(&THREAD_TEST::func2, &t, std::placeholders::_1);
    func(1);
    func2(2);
    t.print_m_a1();
}

TEST(buffer, buf){
    Buffer buf(10);
    Buffer buf1 = buf;
    buf1.write('z');
    std::cout << buf << std::endl;
    std::cout << buf1 << std::endl;
    //
    //    Buffer buf2(buf1);
    //    buf2 = buf1;
    //    std::cout << buf2 << std::endl;
    //    std::cout << buf2 << std::endl;
    //    Buffer* buf3 = &buf1;
    std::cout<<"====="<<std::endl;
    Buffer buf4(10);
    buf4 = Buffer(16);

    std::vector<Buffer> vec_buf;
    vec_buf.emplace_back(10);
    vec_buf.emplace_back(12);

    Buffer buf5{20};

    std::tuple<int,int,int,int,int> t = {1,2,3,4,5};
    println({1, 2, 3, 4});
    print_ln(1,2,3,"4");
}


TEST(redis, test){
    RedisTool r;
    r.init_redis();
    std::string z("ex1");
    std::vector<std::string> a = r.get_all_items(z);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }

    std::string key("ex1");
    std::string value("move6");

    //  zadd
    r.zADD_item(key,kMove, value);
    //  get all
    a = r.get_all_items(z);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }
    //  get type all
    a = r.get_type_items(key,kMove);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }
    std::vector<std::string> items;
    items.emplace_back("move7");
    items.emplace_back("move8");

    //  add vec
    print("%d\n", r.zADD_items(key, kMove, items, 2));
    a = r.get_type_items(key,kMove);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }

    //get size
    print("%d\n", r.get_size(key, kMove));
    print("del %d\n", r.del_type_items(key, kMove));
    print("%d\n", r.get_size(key, kMove));
    print("del %d\n", r.del_key(key));

    a = r.get_all_items(key);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }
}

TEST(json_parse, test){
    std::string json(R"(aaa:[%, %, %] )");
    std::vector<double> a{1.1,2.2,3.21};
    double z = a.at(0);
    replace_json_symbol(json, a.at(0), a.at(1), a.at(2), 6);
    std::cout << json <<std::endl;
}

TEST(string, test){
    std::string a1("test");
    if(a1 == "test"){
        std::cout<< 11111;
    }
}

TEST(pose_parse, test){
    JsonDealClass j;
    RedisTool r;
    r.init_redis();
    char js[4]= "123";
    std::string json(R"(
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
    }     )");
    std::string map_name;
    std::string dbs_value;
    int pos_id;
    std::cout<<std::endl;
    j.parse_chassis_data_to_dbs(json, map_name, pos_id, dbs_value);
    int size = r.get_key_item_size(map_name);

    std::cout << map_name << " " << pos_id << " " << dbs_value <<std::endl << size;

    std::string autojson(R"( { "roadmap": "map_a", "nid": [1,2,3,666] } )");
    std::string m_2;
    std::vector<int> v_2;
    j.parse_automode_param(autojson, m_2, v_2);
    std::cout<< " auto prase " << m_2 <<"  " <<
    v_2.at(0) << " " << v_2.at(1) << " "<< v_2.at(2) << " " << v_2.at(3)<< " ";

}

TEST(parse_dbs, test){
    JsonDealClass j;
    std::vector<std::string> vec_list;
    vec_list.emplace_back("{\n"
                          "      \"function\": \"chs\",\n"
                          "      \"value\": {\n"
                          "        \"xyz\": [\n"
                          "          1.1,\n"
                          "          2.2,\n"
                          "          3.3\n"
                          "        ],\n"
                          "        \"rpy\": [\n"
                          "          1.1,\n"
                          "          2.2,\n"
                          "          3.3\n"
                          "        ],\n"
                          "        \"blh\": null\n"
                          "      }\n"
                          "    }");

    vec_list.emplace_back("{\n"
                          "      \"function\": \"ptz\",\n"
                          "      \"value\": {\n"
                          "        \"cmd\": \"ctl_level_left\",\n"
                          "        \"H\": \"1f\",\n"
                          "        \"L\": \"2b\"\n"
                          "      }\n"
                          "    }");

    vec_list.emplace_back("{\n"
                          "      \"function\": \"ptz\",\n"
                          "      \"value\": {\n"
                          "        \"cmd\": \"ctl_level_right\",\n"
                          "        \"H\": \"1f\",\n"
                          "        \"L\": \"2b\"\n"
                          "      }\n"
                          "    }");

    vec_list.emplace_back("{ \"function\": \"vlc\",\n"
                          "                            \"value\": {\n"
                          "                                  \"cmd\": \"ptz_iris_close_stop\",\n"
                          "                                  \"action\": \"Identify\",\n"
                          "                                  \"pos1\":[ [2.4, 2,4], [3.1, 2.4] ],\n"
                          "                                  \"reserve\": \"etc\"\n"
                          "                                  }\n"
                          "                           }");

    vec_list.emplace_back("{ \"function\": \"irc\",\n"
                          "                            \"value\": {\n"
                          "                                  \"cmd\": \"ptz_iris_close_stop\",\n"
                          "                                  \"action\": \"Identify\",\n"
                          "                                  \"pos1\":[ [2.4, 2,4], [3.1, 2.4] ],\n"
                          "                                  \"reserve\": \"etc\"\n"
                          "                                  }\n"
                          "                           }");

    std::vector<std::string> vec_chs;
    std::vector<std::string> vec_ptz;
    std::vector<std::string> vec_vlc;
    std::vector<std::string> vec_irc;
    std::vector<std::string> vec_chs_cmd;
    std::vector<std::string> vec_ptz_cmd;
    std::vector<std::string> vec_vlc_cmd;
    std::vector<std::string> vec_irc_cmd;
    std::string func_name;

    j.parse_nid_dbs_to_subvec( vec_list,
                               vec_chs_cmd, vec_chs,
                               vec_ptz_cmd, vec_ptz,
                               vec_vlc_cmd, vec_vlc,
                               vec_irc_cmd, vec_irc);

    std::cout <<"zzz"<< vec_chs.size() << " " <<
    vec_ptz.size() << " " <<
    vec_vlc.size() << " " <<
    vec_irc.size() << " " <<
    vec_ptz_cmd.size() << " " <<
    vec_ptz_cmd.size() << " " <<
    vec_ptz_cmd.size() << " " <<
    vec_ptz_cmd.size() << " " ;





    std::cout << vec_chs.at(0) << std::endl;
    std::cout << vec_ptz.at(0) << std::endl;
    std::cout << vec_ptz.at(1) << std::endl;
    std::cout << vec_vlc.at(0) << std::endl;
    std::cout << vec_irc.at(0) << std::endl << std::endl;

    //    std::cout << vec_chs_cmd.at(0) << std::endl;
    std::cout << vec_ptz_cmd.at(0) << std::endl;
    std::cout << vec_ptz_cmd.at(1) << std::endl;
    std::cout << vec_vlc_cmd.at(0) << std::endl;
    std::cout << vec_irc_cmd.at(0) << std::endl;



}

TEST(map, test){
    std::map<std::string, std::string> ptz;
    ptz.insert(std::make_pair("zzzz", "test1"));
    ptz.insert(std::make_pair("zzzz", "test2"));


}

TEST(strtol, test){
    std::string H("aa");
    std::string L("bb");


    long long h = strtol(H.data(), nullptr,16);
    long long l = strtol(L.data(), nullptr,16);

    unsigned short d = h<<8|(l&0xff);
    std::cout << d<<std::endl;
}

TEST(cjsonreplace, test){
    std::string json(R"({
                                  "cmd": "get_a_picture",
                                  "action": "None",
                                  "pos1":[ [-1, -1], [-1, -1] ],
                                  "save_url": "/a/b/c",
                                  "reserve": "etc"
    })");

    cJSON *root = cJSON_Parse(json.data());
    cJSON_ReplaceItemInObject(root, "save_url", cJSON_CreateString("/zzzzzz"));

    std::cout << cJSON_Print(root);
}

TEST(substr,test){
    std::string topic("/a/b/c");
    int index = topic.find_last_of('/');
    std::cout << topic.substr(index+1);

};

#include "rapidjson/reader.h"
using namespace rapidjson;
using namespace std;

TEST(rapidjson, reader){
    struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
        char map_name[64] = {};
        char now_key[16]={};
        unsigned int nid_list[16] = {};
        unsigned int nid_index = 0;


        bool Null() { cout << "Null()" << endl; return true; }
        bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
        bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
        bool Uint(unsigned u) {
            if(strcmp(now_key, "nid") == 0){
                nid_list[nid_index] = u;
                nid_index ++;
                cout << "list!! " <<now_key << nid_index << "nid_list " << nid_list[nid_index]<<endl;
            }
            cout << " Uint(" << u << ")" << endl; return true;
        }
        bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
        bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
        bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
        bool String(const char* str, SizeType length, bool copy) {
            if(strcmp(now_key, "roadmap") == 0){
                memset(map_name, 0, 64);
                memcpy(map_name, str, length);
            }

            cout << "roadmap " << map_name << " String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
            return true;
        }
        bool StartObject() { cout << "StartObject()" << endl; return true; }
        bool Key(const char* str, SizeType length, bool copy) {
            memset(now_key,0,16);
            memcpy(now_key,str, length);
            cout << "Now_key " << now_key << " Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
            return true;
        }
        bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
        bool StartArray() { cout << "StartArray()" << endl; return true; }
        bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
    };

    std::string json(R"({
    "roadmap": "map1",
    "nid_size": 7,
    "nid": [1,2,3,4],
    "timestamp": 2342315234,
    "nid0": [{
        "function": "chs",
        "value": {
            "xyz": [1.1, 2.2, 3.3],
            "rpy": [1.1, 2.2, 3.3],
            "blh": null
        }
    }, {
        "function": "ptz",
        "value": {
            "cmd": "ctl_level_left",
            "H": "1f",
            "L": "2b"
        }
    }, {
        "function": "ptz",
        "value": {
            "cmd": "ctl_level_left",
            "H": "1f",
            "L": "2b"
        }
    }, {
        "function": "vlc",
        "value": {
            "cmd": "get_a_picture",
            "action": "None",
            "pos1": [
                [-1, -1],
                [-1, -1]
            ],
            "save_url": "/a/b/c",
            "reserve": "etc"
        }
    }, {
        "function": "irc",
        "value": {
            "cmd": "ptz_iris_close_stop",
            "action": "Identify",
            "pos1": [
                [2.4, 2, 4],
                [3.1, 2.4]
            ],
            "reserve": "etc"
        }
    }]
    })");

    MyHandler handler;
    Reader reader;
    StringStream ss(json.data());
    reader.Parse(ss, handler);
}

TEST(int_, test){
    int a = 5;
    int b = 2;
    cout << a/b<<endl;
    cout << a%b<<endl;
    string aa = "abc";
    string bb = "zzz";
    if(aa != bb){
        cout << bb <<endl;
    }

}