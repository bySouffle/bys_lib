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

#define AWESOME_MAKE_ENUM(name, ...) enum class name { __VA_ARGS__, __COUNT}; \
inline std::ostream& operator<<(std::ostream& os, name value) { \
std::string enumName = #name; \
std::string str = #__VA_ARGS__; \
int len = str.length(); \
std::vector<std::string> strings; \
std::ostringstream temp; \
for(int i = 0; i < len; i ++) { \
if(isspace(str[i])) continue; \
        else if(str[i] == ',') { \
        strings.push_back(temp.str()); \
        temp.str(std::string());\
        } \
        else temp<< str[i]; \
} \
strings.push_back(temp.str()); \
os << enumName <<"::" << strings[static_cast<int>(value)]; \
return os;}


AWESOME_MAKE_ENUM (type,
                   TYPE1 = 10,
                   TYPE2 = 100
);
class ZE{
public:

};

TEST(enum_in_class, test){
//    ZE zzz;
//    cout << sizeof(zzz);
//    char a = 0x11;
//    string aa = std::to_string(a) ;
//    cout <<aa <<endl;


//    std::stringstream ss;
//    ss << std::hex << (int)a;
//    std::string mystr = ss.str();
//    cout << mystr;
//
    std::stringstream ss;

//    ss << std::setfill('0') << std::hex << std::setw(2) << std::int8_t(-1);
    ss << std::setfill('0') << std::hex << std::setw(2) << (unsigned int)std::int8_t(-1);
    cout <<  ss.str() << "\n";
}

#include "archiver/PtzTaskJsonReader.h"


TEST( ptz_parse_v2, test ){
    struct pelco_d_info
    {
        unsigned char  m_sync ;

        unsigned char  m_address ;

        unsigned char  m_cmd_1 ;
        unsigned char  m_cmd_2 ;

        unsigned char  m_data_1 ;
        unsigned char  m_data_2 ;

        unsigned char  m_check_sum ;

        void Print(std::ostream &os){
            os << "start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";

            os << "m_sync \t"  << std::hex << (int) m_sync << "\n";
            os << "m_address \t"  << std::hex << (int) m_address << "\n";
            os << "m_cmd_1 \t"  << std::hex << (int) m_cmd_1 << "\n";
            os << "m_cmd_2 \t"  << std::hex << (int) m_cmd_2 << "\n";
            os << "m_data_1 \t"  << std::hex << (int) m_data_1 << "\n";
            os << "m_data_2 \t"  << std::hex << (int) m_data_2 << "\n";
            os << "m_check_sum \t"  << std::hex << (int) m_check_sum << "\n";
            
            os << "end   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
            os << endl;

        }
    };

    auto gen_device_param = [](std::array<unsigned char, 2> &device_param, int hex_angle_x100){
        device_param[0] = (hex_angle_x100 >> 8)& 0xff;
        device_param[1] = (hex_angle_x100     )& 0xff;
    };

    auto calc_msg_chksum = [](pelco_d_info *frame, int param_size)->int{
        if( frame == nullptr ){
        return -1;
        }

        char * data = (char*)frame;
        unsigned char ret_val = 0;
        for(int i = 1; i <= param_size; ++i){
            ret_val += data[i];
        }

        frame->m_check_sum = ret_val & 0xFF;
        return ret_val;
    };

    auto calc_pix_offset_angle = [](int x_offset, int y_offset, double& angle_x, double& angle_y)
    {
        angle_x = x_offset/32.00;
        angle_y = y_offset/32.00;
    };

    {
        PtzTaskJson ptz_data;
        JsonReader reader(R"({
        "timestamp": 202101172047,
        "function": "ptz",
        "value": {
            "cmd": "auto_exec",
            "level_angle": 45,
            "pitch_angle": 90,
            "zoom_value": 180,
            "focus_value": 360,
            "save_url": "a/c/v"
        }
        })");

        reader & ptz_data;
        ptz_data.Print(cout);

        pelco_d_info frame_info{0xff, 0x01, 0, 0, 0, 0, 0} ;
        //  1>> 移动云台
        //  level cmdbit
        frame_info.m_cmd_1 = 0x00;
        frame_info.m_cmd_2 = 0x4B;
        std::array<unsigned char, 2> level_data{};
        ptz_data.gen_device_param( level_data, PtzTaskJson::kLevel );
        frame_info.m_data_1 = level_data[0];
        frame_info.m_data_2 = level_data[1];
        calc_msg_chksum(&frame_info, 5);
        frame_info.Print(cout);

        //  pitch cmdbit
        frame_info.m_cmd_1 = 0x00;
        frame_info.m_cmd_2 = 0x4D;
        std::array<unsigned char, 2> pitch_data{};
        ptz_data.gen_device_param( pitch_data, PtzTaskJson::kPitch );
        frame_info.m_data_1 = pitch_data[0];
        frame_info.m_data_2 = pitch_data[1];
        calc_msg_chksum(&frame_info, 5);
        frame_info.Print(cout);

        //  2>> 计算像素偏移
        int offset_x{};
        int offset_y{};

        double angle_x{};
        double angle_y{};

        string tmp_pic("/opt/move_tmp.jpg");

        offset_x = 33;
        offset_y = 65;
//        if( light_api->fun_get_a_picture(tmp_pic) == 0 ){
//            cv::Mat cur_pic = cv::imread(tmp_pic.toUtf8().data());
//            cv::Mat original_pic = cv::imread( ptz_data.getSave_url() );
//            CVObjTracking cc;
//            cc.offset_detection(original_pic, cur_pic, &offset_x, &offset_y);
//
//            offset_x = 33;
//            offset_y = 65;
//        }
        cout << "offset " << std::dec << offset_x <<" " << offset_y <<"\n";
        calc_pix_offset_angle(offset_x, offset_y, angle_x, angle_y);
        int move_l = angle_x*100 + ptz_data.getLevel_angle()*100;
        int move_p = angle_y*100 + ptz_data.getPitch_angle()*100;
        cout << " getLevel_angle " << ptz_data.getLevel_angle()
        << " getPitch_angle " << ptz_data.getPitch_angle();

        cout << "offset " << std::dec << angle_x <<" " << angle_y <<"\n"
        <<  move_l << " " <<move_p << "\n";

        //  二次校准调整云台
        //      level
        frame_info.m_cmd_1 = 0x00;
        frame_info.m_cmd_2 = 0x4B;
        ptz_data.gen_device_param(level_data, move_l) ;
        frame_info.m_data_1 = level_data[0];
        frame_info.m_data_2 = level_data[1];
        calc_msg_chksum(&frame_info, 5);
        frame_info.Print(cout);

        //      pitch
        frame_info.m_cmd_1 = 0x00;
        frame_info.m_cmd_2 = 0x4D;
        ptz_data.gen_device_param(pitch_data, move_p);
        frame_info.m_data_1 = pitch_data[0];
        frame_info.m_data_2 = pitch_data[1];
        calc_msg_chksum(&frame_info, 5);
        frame_info.Print(cout);

        //  3>> 调整倍率焦距
        //      zoom
        frame_info.m_cmd_1 = 0x00;
        frame_info.m_cmd_2 = 0x4F;
        ptz_data.gen_device_param(pitch_data, PtzTaskJson::kZoom);
        frame_info.m_data_1 = pitch_data[0];
        frame_info.m_data_2 = pitch_data[1];
        calc_msg_chksum(&frame_info, 5);
        frame_info.Print(cout);

         //      focus
        frame_info.m_cmd_1 = 0x00;
        frame_info.m_cmd_2 = 0x6F;
        ptz_data.gen_device_param(pitch_data, PtzTaskJson::kFocus);
        frame_info.m_data_1 = pitch_data[0];
        frame_info.m_data_2 = pitch_data[1];
        calc_msg_chksum(&frame_info, 5);
        frame_info.Print(cout);

    }
}

#include <numeric>

TEST(zzzand, test){
    int a = 10;  //  8 4 2 1
    int b = (a&0x8);
    cout << b << endl;

    std::array<unsigned char, 7> level_cmd_ = {1,0xff,3,4,5,6,7};
    unsigned char zzz = std::accumulate(level_cmd_.begin()+1, level_cmd_.end()-1, 0);
    printf("%d\n", zzz);

}