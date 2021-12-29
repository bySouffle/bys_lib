//
// Created by bysou on 2021/9/21.
//

#include "RedisTool.h"
#include "JsonDealClass.h"
#include "gtest/gtest.h"
#include "TempClass.h"
#include "Buffer.h"

#include "rapidjson/reader.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;

TEST(hellow, out) {
    printf("hello wolrd\r\n");
}

TEST(bind, out) {
    THREAD_TEST t;
    auto func = std::bind(&THREAD_TEST::func1, &t, std::placeholders::_1);
    auto func2 = std::bind(&THREAD_TEST::func2, &t, std::placeholders::_1);
    func(1);
    func2(2);
    t.print_m_a1();
}

TEST(buffer, buf) {
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
    std::cout << "=====" << std::endl;
    Buffer buf4(10);
    buf4 = Buffer(16);

    std::vector<Buffer> vec_buf;
    vec_buf.emplace_back(10);
    vec_buf.emplace_back(12);

    Buffer buf5{20};

    std::tuple<int, int, int, int, int> t = {1, 2, 3, 4, 5};
    println({1, 2, 3, 4});
}


TEST(redis, test) {
    RedisTool r;
    r.init_redis();
    std::string z("ex1");
    std::vector<std::string> a = r.get_all_items(z);
    for (auto aa: a) {
        std::cout << aa << std::endl;
    }

    std::string key("ex1");
    std::string value("move6");

    //  zadd
    r.zADD_item(key, kMove, value);
    //  get all
    a = r.get_all_items(z);
    for (auto aa: a) {
        std::cout << aa << std::endl;
    }
    //  get type all
    a = r.get_type_items(key, kMove);
    for (auto aa: a) {
        std::cout << aa << std::endl;
    }
    std::vector<std::string> items;
    items.emplace_back("move7");
    items.emplace_back("move8");

    //  add vec
    print("%d\n", r.zADD_items(key, kMove, items, 2));
    a = r.get_type_items(key, kMove);
    for (auto aa: a) {
        std::cout << aa << std::endl;
    }

    //get size
    print("%d\n", r.get_size(key, kMove));
    print("del %d\n", r.del_type_items(key, kMove));
    print("%d\n", r.get_size(key, kMove));
    print("del %d\n", r.del_key(key));

    a = r.get_all_items(key);
    for (auto aa: a) {
        std::cout << aa << std::endl;
    }
}

TEST(json_parse, test) {
    std::string json(R"(aaa:[%, %, %] )");
    std::vector<double> a{1.1, 2.2, 3.21};
    double z = a.at(0);
    replace_json_symbol(json, a.at(0), a.at(1), a.at(2), 6);
    std::cout << json << std::endl;
}

TEST(string, test) {
    std::string a1("test");
    if (a1 == "test") {
        std::cout << 11111;
    }
}

TEST(pose_parse, test) {
    JsonDealClass j;
    RedisTool r;
    r.init_redis();
    char js[4] = "123";
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
    std::cout << std::endl;
    j.parse_chassis_data_to_dbs(json, map_name, pos_id, dbs_value);
    int size = r.get_key_item_size(map_name);

    std::cout << map_name << " " << pos_id << " " << dbs_value << std::endl << size;

    std::string autojson(R"( { "roadmap": "map_a", "nid": [1,2,3,666] } )");
    std::string m_2;
    std::vector<int> v_2;
    j.parse_automode_param(autojson, m_2, v_2);
    std::cout << " auto prase " << m_2 << "  " <<
              v_2.at(0) << " " << v_2.at(1) << " " << v_2.at(2) << " " << v_2.at(3) << " ";

}

TEST(parse_dbs, test) {
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

    j.parse_nid_dbs_to_subvec(vec_list,
                              vec_chs_cmd, vec_chs,
                              vec_ptz_cmd, vec_ptz,
                              vec_vlc_cmd, vec_vlc,
                              vec_irc_cmd, vec_irc);

    std::cout << "zzz" << vec_chs.size() << " " <<
              vec_ptz.size() << " " <<
              vec_vlc.size() << " " <<
              vec_irc.size() << " " <<
              vec_ptz_cmd.size() << " " <<
              vec_ptz_cmd.size() << " " <<
              vec_ptz_cmd.size() << " " <<
              vec_ptz_cmd.size() << " ";


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

TEST(map, test) {
    std::map<std::string, std::string> ptz;
    ptz.insert(std::make_pair("zzzz", "test1"));
    ptz.insert(std::make_pair("zzzz", "test2"));


}

TEST(strtol, test) {
    std::string H("aa");
    std::string L("bb");


    long long h = strtol(H.data(), nullptr, 16);
    long long l = strtol(L.data(), nullptr, 16);

    unsigned short d = h << 8 | (l & 0xff);
    std::cout << d << std::endl;
}

TEST(cjsonreplace, test) {
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

TEST(substr, test) {
    std::string topic("/a/b/c");
    int index = topic.find_last_of('/');
    std::cout << topic.substr(index + 1) << std::endl;
    std::string topic1("/z/z");
    topic1.append(topic.substr(topic.find_last_of('/')).data());
    std::cout << topic1 << std::endl;
};



TEST(rapidjson, reader) {
    struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
        char map_name[64] = {};
        char now_key[16] = {};
        unsigned int nid_list[16] = {};
        unsigned int nid_index = 0;


        bool Null() {
            cout << "Null()" << endl;
            return true;
        }

        bool Bool(bool b) {
            cout << "Bool(" << boolalpha << b << ")" << endl;
            return true;
        }

        bool Int(int i) {
            cout << "Int(" << i << ")" << endl;
            return true;
        }

        bool Uint(unsigned u) {
            if (strcmp(now_key, "nid") == 0) {
                nid_list[nid_index] = u;
                nid_index++;
                cout << "list!! " << now_key << nid_index << "nid_list " << nid_list[nid_index] << endl;
            }
            cout << " Uint(" << u << ")" << endl;
            return true;
        }

        bool Int64(int64_t i) {
            cout << "Int64(" << i << ")" << endl;
            return true;
        }

        bool Uint64(uint64_t u) {
            cout << "Uint64(" << u << ")" << endl;
            return true;
        }

        bool Double(double d) {
            cout << "Double(" << d << ")" << endl;
            return true;
        }

        bool String(const char *str, SizeType length, bool copy) {
            if (strcmp(now_key, "roadmap") == 0) {
                memset(map_name, 0, 64);
                memcpy(map_name, str, length);
            }

            cout << "roadmap " << map_name << " String(" << str << ", " << length << ", " << boolalpha << copy << ")"
                 << endl;
            return true;
        }

        bool StartObject() {
            cout << "StartObject()" << endl;
            return true;
        }

        bool Key(const char *str, SizeType length, bool copy) {
            memset(now_key, 0, 16);
            memcpy(now_key, str, length);
            cout << "Now_key " << now_key << " Key(" << str << ", " << length << ", " << boolalpha << copy << ")"
                 << endl;
            return true;
        }

        bool EndObject(SizeType memberCount) {
            cout << "EndObject(" << memberCount << ")" << endl;
            return true;
        }

        bool StartArray() {
            cout << "StartArray()" << endl;
            return true;
        }

        bool EndArray(SizeType elementCount) {
            cout << "EndArray(" << elementCount << ")" << endl;
            return true;
        }
    };

    std::string json(R"({
    "roadmap": "map1",
    "nid_size": 7,
    "nid": [1,2,3,4],
    "timestamp": 2342315234,
    "nid0": [{
        "function": "chs",
        "value": {
            "timestamp": 2342315234,
            "xyz": [1.1, 2.2, 3.3],
            "rpy": [1.1, 2.2, 3.3],
            "blh": null
        }
    }, {
        "timestamp": 2342315235,
        "function": "ptz",
        "value": {
            "cmd": "ctl_level_left",
            "H": "1f",
            "L": "2b"
        }
    }, {
        "timestamp": 2342315238,
        "function": "ptz",
        "value": {
            "cmd": "ctl_level_left",
            "H": "1f",
            "L": "2b"
        }
    }, {
        "timestamp": 2342315239,
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
        "timestamp": 2342315240,
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

/*
 *     "status":"done",        //TODO add
 * */
TEST(map_to_dbs, test) {
    JsonDealClass j;
    std::string json(R"({
	"roadmap": "map1",
	"nid_size": 7,
	"nid": [
		0,
		1,
		2,
		3
	],
	"timestamp": 2342315234,
    "status":"done",

	"nid0": [{
            "timestamp": 2342315241,
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
            "timestamp": 2342315242,
			"function": "ptz",
			"value": {
				"cmd": "ctl_level_left",
				"H": "1f",
				"L": "2b"
			}
		},
		{
            "timestamp": 2342315243,
			"function": "ptz",
			"value": {
				"cmd": "ctl_level_left",
				"H": "1f",
				"L": "2b"
			}
		},
		{
            "timestamp": 2342315244,
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
		},
		{
            "timestamp": 2342315245,
			"function": "irc",
			"value": {
				"cmd": "ptz_iris_close_stop",
				"action": "Identify",
				"pos1": [
					[
						2.4,
						2,
						4
					],
					[
						3.1,
						2.4
					]
				],
				"reserve": "etc"
			}
		}
	],
	"nid1": [{
            "timestamp": 2342315246,
			"function": "chs",
			"value": {
				"xyz": [
					1.2,
					2.2,
					3.3
				],
				"rpy": [
					1.2,
					2.2,
					3.3
				],
				"blh": null
			}
		},
		{
            "timestamp": 2342315252,
			"function": "ptz",
			"value": {
				"cmd": "ctl_level_left",
				"H": "1d",
				"L": "2b"
			}
		},
		{
            "timestamp": 2342315260,
			"function": "ptz",
			"value": {
				"cmd": "ctl_level_left",
				"H": "1z",
				"L": "2b"
			}
		},
		{
            "timestamp": 2342315265,
			"function": "vlc",
			"value": {
				"cmd": "get_a_picture",
				"action": "None",
				"pos1": [
					[-1, -2],
					[-1, -1]
				],
				"save_url": "/a/b/c",
				"reserve": "etc"
			}
		},
		{
            "timestamp": 2342315280,
			"function": "irc",
			"value": {
				"cmd": "ptz_iris_close_stop",
				"save_url": "/a/b/c",
				"pos1": [1.1,2,2],
                "temperature":3.14,
				"reserve": "etc"
			}
		}
	]
    })");
    j.save_map_2_dbs(json);
}

TEST(redis_get_all, test) {
    RedisTool r;
    std::string s("map1");
//    r.get_all_items(s);
    r.get_type_items(s, 0);
}

TEST(parse_reids_to_json, test) {
    JsonDealClass j;

    std::string cmd(R"({
	"cmd": "get_map",
	"mapname": "map1",
	"nid": [0, 1]
    })");
    std::string json;
    j.from_db_gen_map_json(cmd, json);
}


/*
 * {
            "cmd": "get_a_picture",
            "action": "None",
            "pos1": [
                [-1, -1],
                [-1, -1]
            ],
            "save_url": "/a/b/c",
            "reserve": "etc"
        }
 *
 * */
TEST(cjson_createarr, test) {
    cJSON *root = cJSON_CreateObject();
    double pos1[2] = {-1, -1};
    double pos2[2] = {-1, -1};

    cJSON *pos_arr = cJSON_CreateArray();

    cJSON *pos1_item = cJSON_CreateDoubleArray(pos1, 2);
    cJSON *pos2_item = cJSON_CreateDoubleArray(pos1, 2);

    cJSON_AddItemToArray(pos_arr, pos1_item);
    cJSON_AddItemToArray(pos_arr, pos2_item);
    cJSON_AddItemToObject(root, "pos", pos_arr);
    cout << cJSON_Print(root);
    cJSON_Delete(root);
}

TEST(cases, test) {

    int cmd = 1;
    switch (cmd) {
        case 0 : {
            printf("%d \r\n", __LINE__);
        }
        case 1 : {
            printf("%d \r\n", __LINE__);

            cmd = 3;
            goto exitlab;
        }
        case 2 : {
            printf("%d \r\n", __LINE__);
        }

        case 3 : {
            cmd = 2;

            printf("%d \r\n", __LINE__);
        }
        case 4 : {
            printf("%d \r\n", __LINE__);
        }
            break;
        case 9 : {
            printf("%d \r\n", __LINE__);
        }
            break;
    }

    exitlab:
    printf("Zzzzzzzzzzzzzzzzzzzzzzzzz\n");
}

TEST(char_short, test) {
    unsigned char a[12] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
                           0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc};

    unsigned short b[24] = {0x1122, 0x3344};

    memcpy(b, a, 24);
    printf("b[0x%x] [0x%x] [0x%x] [0x%x]\n", b[0], b[1], b[2], b[3]);

    auto f = [](unsigned short *c) {
        printf("\n[0x%x]\n", c[0]);
        unsigned char *cc = (unsigned char *) c;
        printf("cc [0x%x] [0x%x] [0x%x]\n", cc[0], cc[1], cc[2]);
    };

//    f((unsigned short *)a);
    f(b);
    unsigned short *c = (unsigned short *) a;
    printf("\nb[0x%x]\n", (unsigned short) c[1]);
}


TEST(litte_end, test) {
    unsigned short a[5] = {0x1122, 0x3344, 0x5566, 0x7788, 0x99aa};
    for (int i = 0; i < 5; ++i) {
        a[i] = (a[i] >> 8) | (a[i] << 8);
    }
    printf("[0x%x] [0x%x] [0x%x] [0x%x] [0x%x] \n", a[0], a[1], a[2], a[3], a[4]);

}

TEST(vlc_parse, test) {
    string bejson(R"({
				"cmd": "get_a_picture",
				"action": "None",
				"pos": [
					[-1, -2],
					[-3, -4]
				],
				"save_url": "/a/b/c",
				"reserve": "etc"
			})");

    string p_json(R"({"cmd":"%","action":"%","pos1":[[%,%],[%,%]],"save_url":"%","reserve":"etc"})");
    cJSON *root;
    root = cJSON_Parse(bejson.data());
    if (root != nullptr) {
        //  1. parse cmd
        cJSON *k_cmd = cJSON_GetObjectItem(root, "cmd");
        if (k_cmd != nullptr) {
            replace_json_symbol_str(p_json, cJSON_GetStringValue(k_cmd));
        } else {
            replace_json_symbol_str(p_json, "known");
        }
        //  2. parse action
        cJSON *k_action = cJSON_GetObjectItem(root, "action");
        if (k_cmd != nullptr) {
            replace_json_symbol_str(p_json, cJSON_GetStringValue(k_action));
        } else {
            replace_json_symbol_str(p_json, "None");
        }
        //  3. parse pos
        cJSON *k_pos = cJSON_GetObjectItem(root, "pos");
        if ((k_pos != nullptr) && cJSON_IsArray(k_pos)) {
            for (int i = 0; i < cJSON_GetArraySize(k_pos); ++i) {
                cJSON *pos1 = cJSON_GetArrayItem(k_pos, i);
                if ((pos1 != nullptr) && cJSON_IsArray(pos1)) {
                    for (int j = 0; j < cJSON_GetArraySize(pos1); ++j) {
                        replace_json_symbol(p_json, cJSON_GetArrayItem(pos1, j)->valuedouble);
                    }
                } else {
                    //  ADD 再没有解析到字段时
                    replace_json_symbol(p_json, -1, -1);
                }
            }

        } else {
            replace_json_symbol_str(p_json, "None");
        }
        cJSON_Delete(root);
    }
}

TEST(parse_dbs_to_vec, test) {
    {
        string json(R"(
{
	"timestamp":	2342315246,
	"function":	"chs",
	"value":	{
		"xyz":	[1.2, 3.2, 3.3],
		"rpy":	[1.2, 2.2, 3.3],
		"blh":	null
	}
}
)");

        cJSON *root = cJSON_Parse(json.data());
        if (root != nullptr) {
            cJSON *func_item = cJSON_GetObjectItem(root, "function");
            if (func_item != nullptr) {
                std::string func_value(cJSON_GetStringValue(func_item));
                cout << func_value << endl;

                if (func_value == "chs") {
                    std::string str_payload(R"({"timestamp":"%","args":[{"roadmap":"%","nid":[],"coord":[[%,%]]}]})");

                    double x = 0;
                    double y = 0;
                    {
                        cJSON *val_item = cJSON_GetObjectItem(root, "value");
                        if (val_item != nullptr) {
                            cJSON *xyz_item = cJSON_GetObjectItem(val_item, "xyz");
                            if ((xyz_item != nullptr) &&
                                cJSON_IsArray(xyz_item)
                                    ) {
                                x = cJSON_GetArrayItem(xyz_item, 0)->valuedouble;
                                y = cJSON_GetArrayItem(xyz_item, 1)->valuedouble;
                            }
                        }
                        long timestamp = get_now_time_dec();
                        string map_name("map_a");
                        replace_json_symbol(str_payload, timestamp);
                        replace_json_symbol_str(str_payload, map_name);
                        replace_json_symbol(str_payload, x, y);
                        cout << str_payload << endl;
                    }
                } else {
                    cJSON *value_item = cJSON_GetObjectItem(root, "value");
                    if (value_item != nullptr) {
                        cJSON *k_cmd = cJSON_GetObjectItem(value_item, "cmd");
                        cout << cJSON_GetStringValue(k_cmd) <<endl;
//                        _item.m_func_topic = cJSON_GetStringValue(k_cmd);

                        std::string value_payload(cJSON_Print(value_item));
                        cout<< value_payload <<endl;
//                        _item.m_payload = value_payload;
                    }
//                    i_active_list.emplace_back(_item);
                }
            }
            cJSON_Delete(root);
        }
    }
}

TEST(replace_char, test){
    string s(R"(a=%, b=%)");
//    replace_json_symbol_str(s,"a","b");
    unsigned char h = 0x11;
    cout << s;
    unsigned  char l = 0x22;

    char ret_payload[64] = {};
    sprintf(ret_payload, "{\"cmd\": \"pitch_angle_reply\",\"H\": \"%x\",\"L\": \"%x\" }", h,l );
    cout << ret_payload;
}

TEST(get_now_time, test){
    time_t now;
    time(&now);
    struct tm *t;
    t = localtime(&now);
    cout << t->tm_year+1900 <<" " << t->tm_mon+1 << " " <<t->tm_mday << " " <<t->tm_hour << " "
         << t->tm_min << " " << t->tm_sec;
}

TEST(value_replace, test){
    string json(R"( { "cmd":"zzz", "H":"22", "L":"33" } )");
    cJSON *root = cJSON_Parse(json.data());
    cJSON_ReplaceItemInObject(root, "H", cJSON_CreateString("AA"));
    cJSON_ReplaceItemInObject(root, "L", cJSON_CreateString("BB"));
    json.clear();
    json.append(cJSON_Print(root));
    cJSON_Delete(root);
    cout << json <<endl;

}


TEST(redis_get_score_value, test){
    RedisTool r;
    string key("map1");
    std::multimap<int, std::string> result = r.get_all_scores_key(key);

    for(auto & it : result){
        cout << it.first << ": " << it.second<<endl;
    }
    cout <<"=============================================\n";
    for(auto it = result.begin();it!=result.end();it++){
        cout << it->first << ": " << it->second<<endl;
    }

}

TEST(redis_get_score_value2, test){
    JsonDealClass d;

    string mapname(R"({"roadmap":"map1", "nid":[]})");
    string result;
    d.from_db_gen_map_all_json(mapname, result);

}

TEST(arrive_parse, test){
    JsonDealClass d;
    string s(R"({ "args":[{"status":"ok"}] })");
    d.arrive_parse(s);
}

TEST(rapidjson, write){
    using namespace rapidjson;
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("os");
    {
        writer.StartObject();
        writer.Key("sysname");
        writer.String("Linux");
        writer.Key("nodename");
        writer.String("siasun-NUC8i7BEH");
        writer.EndObject();
    }
    writer.Key("mac");
    writer.String("1C-69-7A-6B-E8-18");
    {
        writer.Key("mac");
        writer.StartArray();
        writer.Int(16262784);
        writer.Int(12159080);
        writer.EndArray();
    }
    writer.EndObject();
    cout<< s.GetString()<<endl;
    s.Clear();
    cout<< " " <<s.GetString() <<" ";

}

TEST(rapidjson, write2){

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    //  1. 获取os信息
#if 0
    p_this->s_->GetSysInfo(u);
        printf(">>>>>>>>>>>>>> os info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [sysname %s]\n", u->sysname);
        printf("2 [nodename %s]\n", u->nodename);
        printf("3 [release %s]\n", u->release);
        printf("4 [version %s]\n", u->version);
        printf("5 [machine %s]\n", u->machine);
        printf("6 [domainname %s]\n", u->domainname);
        printf("<<<<<<<<<<< os info  end <<<<<<<<<<<<<<<\n" );
#endif
    writer.Key("os");
    {

        writer.StartObject();
        writer.Key("sysname");
        writer.String("u->sysname");
        writer.Key("nodename");
        writer.String("u->nodename");
        writer.Key("release");
        writer.String("u->release");
        writer.Key("version");
        writer.String("u->version");
        writer.Key("arch");
        writer.String("u->machine");
        writer.Key("domainname");
        writer.String("u->domainname");
        writer.EndObject();
    }

    //  2. 获取MAC
#if 0
    mac_.append( p_this->s_->GetSysMac() ) ;
        printf(">>>>>>>>>>>>>> mac info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [mac %s]\n", mac_.data());
        printf("<<<<<<<<<<< mac info  end <<<<<<<<<<<<<<<\n" );
#endif
    writer.Key("mac");
    writer.String("mac_.data()");

    //  3. 获取CPU
//    cpu_usage_ = p_this->s_->GetTotalCpuPercent();
#if 0
    printf(">>>>>>>>>>>>>> cpu info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [cpu %.4f]\n", cpu_usage_);
        printf("<<<<<<<<<<< cpu info  end <<<<<<<<<<<<<<<\n" );
#endif
    writer.Key("cpu_usage");
    writer.Double(12.22);

    //  4. 获取内存
//    p_this->s_->GetMemInfo(*mem_info_);
#if 0
    printf(">>>>>>>>>>>>>> mem info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [total %d]\n", mem_info_->total);
        printf("1 [free %d]\n", mem_info_->free);
        printf("1 [buffers %d]\n", mem_info_->buffers);
        printf("1 [cached %d]\n", mem_info_->cached);
        printf("<<<<<<<<<<< mem info  end <<<<<<<<<<<<<<<\n" );
#endif
    {
        writer.Key("mem_info");
        writer.StartArray();
        writer.Int(11);
        writer.Int(22);
        writer.EndArray();
    }

    //  5. 获取硬盘信息
//    p_this->s_->GetDiskPercent(disk_info_);
#if 0
    printf(">>>>>>>>>>>>>> disk info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [total %d GB]\n", disk_info_->total);
        printf("1 [free %d GB]\n", disk_info_->free);
        printf("1 [used %d GB]\n", disk_info_->used);
        printf("1 [Usage %.2f ]\n", disk_info_->Usage);
        printf("<<<<<<<<<<< disk info  end <<<<<<<<<<<<<<<\n" );
#endif
    {
        writer.Key("disk_info");
        writer.StartArray();
        writer.Int(11);
        writer.Int(22);
        writer.Int(33);
        writer.Double(11.22);
        writer.EndArray();
    }
    writer.EndObject();

    cout << s.GetString();
}

TEST(trans_topic, test){
    string s("/aaa/bb/ccc");
    string ret;


//    int end = ret.find_first_of('/', 1);
//    s.substr(1, end-1);
//    s
    int start = 0;
    int end = 0;
    vector<string> vec;

    for (int i = 0; i < 3; ++i) {
        end = s.find_first_of('/', start + 1);
        vec.emplace_back(s.substr(start, end - start));
        start = end;
    }
    ret.append(vec.at(1)).append(vec.at(0)).append(vec.at(2));
    cout << ret;

}

TEST(zzz,zzz){
    cout << 100/11 <<endl;
}

TEST(rapidjson, create){
    Document rpdjson;
    Pointer("/cmd").Set(rpdjson, "get_a_picture");
    Pointer("/save_url").Set(rpdjson, "url.data()");
    Pointer("/pos/0").Set(rpdjson,3.002);
    Pointer("/pos/1").Set(rpdjson,4.02);
    Pointer("/temperature").Set(rpdjson,5.002);
    Pointer("/index/0").Set(rpdjson,1);
    Pointer("/index/1").Set(rpdjson,2);

    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    rpdjson.Accept(writer);

    if(rpdjson["cmd"] == "get_a_picture"){
        cout<<"1zzzzz\n";
    }
    cout << strbuf.GetString() << endl;

}
string getStringFromJson(const string &jsStr, const string &strKey)
{
    Document document;
    if (document.Parse(jsStr.c_str()).HasParseError() || !document.HasMember(strKey.c_str()))
    {
        return "";
    }
    const rapidjson::Value &jv = document[strKey.c_str()];
    return jv.GetString();
};

TEST(goto_reply, create){


    string goto_reply(R"({"args":[{"status":"ok"}]})");
    Document document;
    document.Parse(goto_reply.data());
    const Value& args = document["args"];
    assert(args.IsArray());
    cout << args[0]["status"].GetString();









//
//    StringBuffer strbuf;
//    Writer<StringBuffer> writer(strbuf);
//    rpdjson.Accept(writer);
//
//    if(rpdjson["cmd"] == "get_a_picture"){
//        cout<<"1zzzzz\n";
//    }
//    cout << strbuf.GetString() << endl;

}