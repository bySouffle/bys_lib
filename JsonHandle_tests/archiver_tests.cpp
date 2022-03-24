//
// Created by bysou on 2021/12/25.
//

#include <utility>

#include "gtest/gtest.h"
#include "Archiver/archiver.h"
#include "Archiver/archiver_obj.h"
#include "Archiver/PtzTaskJsonReader.h"


TEST(reader, test){
    Student s;
    JsonReader reader(R"({"name":"zzz", "age":18, "height":175, "canSwim":false})");
    reader &s;
    std::cout << s << std::endl;
}

TEST(writer, test){
    Student s("zzz", 18, 175.5, true);
    JsonWriter writer;
    writer &s;
    std::cout << writer.GetString() << std::endl;
}

TEST(writer_reader_arr, test){
    std::string json;
    {
        Group g;
        g.groupName = "Rainbow";
        Student s1("z1", 18, 175.5, true);
        Student s2("z2", 19, 165.1, true);
        g.students.emplace_back(s1);
        g.students.emplace_back(s2);

        JsonWriter writer;
        writer & g;
        json = writer.GetString();
        std::cout << writer.GetString() << std::endl;
    }
    {
        Group g;
        JsonReader reader(json.c_str());
        reader & g;
        std::cout << g <<std::endl;
    }

}

TEST(polymorphism_and_friend, test){
    std::string json;

    // Serialize
    {
        Canvas c;
        c.AddShape(new Circle(1.0, 2.0, 3.0));
        c.AddShape(new Box(4.0, 5.0, 6.0, 7.0));

        JsonWriter writer;
        writer & c;
        json = writer.GetString();
        std::cout << json << std::endl;
    }

    // Deserialize
    {
        Canvas c;
        JsonReader reader(json.c_str());
        reader & c;
        c.Print(std::cout);
    }
}

////////////////////////////////////////////////////////////////////////////////
class SysInfo{
public:
    ~SysInfo();
    void Print(std::ostream& os);
    SysInfo(){};
    SysInfo(std::string  version,std::string  release,std::vector<int> disk_info,
            std::string  mac);
protected:

    template<typename Archiver>
    friend Archiver& operator&(Archiver& ar, SysInfo& s);

    std::string linux_version;
    std::string release;
    std::vector<int>disk_info;
    std::string mac;

};

template<typename Archiver>
Archiver& operator&(Archiver& ar, SysInfo& s){
    ar.StartObject();

    ar.Member("os") ;
    ar.StartObject();   //  os start
    ar.Member("linux_version") & s.linux_version;
    ar.Member("release") & s.release;
    ar.EndObject();     //  os end

    ar.Member("base_info");
    ar.StartObject();   //  base_info start
    ar.Member("disk_info");
    size_t cnt = s.disk_info.size();
    ar.StartArray(&cnt);
    if(ar.IsReader)   s.disk_info.resize(cnt);
    for (size_t i = 0; i < cnt; ++i) {
        ar&s.disk_info[i];
    }
    ar.EndArray();
    ar.Member("mac") & s.mac;
    ar.EndObject();     //  base_info   end

    return ar.EndObject();

}

SysInfo::SysInfo(std::string version, std::string release, std::vector<int> disk_info,
                 std::string mac):linux_version(std::move(version)), release(std::move(release)),disk_info(std::move(disk_info)),
                                         mac(std::move(mac)) {

}

SysInfo::~SysInfo() {

}

void SysInfo::Print(std::ostream &os) {
    os << "version: " << this->linux_version << "\nrelease: " << this->release;
    os << std::endl;

    os << "disk_info: ";
    for (auto &it : this->disk_info) {
        os << it << " ";
    }
    os << "mac: " << this->mac;
    os << std::endl;


};

TEST(gen_json, test){
    std::string linux_version("ubuntu");
    std::string release("v5.14.2");
    std::vector<int>disk_info{100,99,1};
    std::string mac("00-00-00-00-00-00");
    SysInfo s(linux_version, release, disk_info, mac  );
    JsonWriter writer;
    writer & s;
    std::cout << writer.GetString()<<std::endl;

    std::string jjson(R"({
    "os": {
        "linux_version": "ubuntu",
        "release": "v5.14.2"
    },
    "base_info": {
        "disk_info": [
            100,
            99,
            1
        ],
        "mac": "00-00-00-00-00-00"
    }
})");
    JsonReader reader(jjson.c_str());
    SysInfo ss;
    reader & ss;
    ss.Print(std::cout);



}
////////////////////////////////////////////////////////////////////////////////

TEST(ptz, data_write){
    uint64_t timestamp = 202101172047;
    std::string function("ptz");
    std::string cmd("auto_exec");
    unsigned  level_angle = 45;
    unsigned  pitch_angle = 90;
    unsigned  zoom_value = 180;
    unsigned  focus_value = 360;
    std::string save_url("a/c/v");

    PtzTaskJson data(timestamp, function, cmd, level_angle, pitch_angle, zoom_value, focus_value,
                     save_url);
    JsonWriter writer;
    writer & data;
    std::cout << writer.GetString() << "\n";

    JsonReader reader(writer.GetString() );
    PtzTaskJson read_data;
    reader &read_data;

    read_data.Print(std::cout);

    std::array<unsigned char, 2> arr{};
    read_data.gen_device_param(arr, PtzTaskJson::kLevel);
    for (auto& it: arr) {
        printf("0x%x\n", it);
    }
    std::cout << std::endl;
    read_data.gen_device_param(arr, PtzTaskJson::kPitch);
    for (auto& it: arr) {
        printf("0x%x\n", it);
    }
    std::cout << std::endl;
    read_data.gen_device_param(arr, PtzTaskJson::kZoom);
    for (auto& it: arr) {
        printf("0x%x\n", it);
    }
    std::cout << std::endl;
    read_data.gen_device_param(arr, PtzTaskJson::kFocus);
    for (auto& it: arr) {
        printf("0x%x\n", it);
    }
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

#include "Capitalize/capitalize.h"
TEST(toupper_json, capitalize){
    using namespace rapidjson;
        // Prepare JSON reader and input stream.
    Reader reader;
    char readBuffer[65536];

    FILE *read_fp = fopen("/Users/bysouffle/CLionProjects/bys_lib/JsonHandle_tests/test_json.txt", "r");
    //!     1. 初始化读取文件流对象
    FileReadStream is(read_fp, readBuffer, sizeof(readBuffer));
    std::cout << readBuffer << "\n";

    // Prepare JSON writer and output stream.
    char writeBuffer[65536];
    FILE * write_fp = fopen("/Users/bysouffle/CLionProjects/bys_lib/JsonHandle_tests/test_json_upper.txt",
                            "w");

    //!     2. 初始化待写入的文件流对象
    FileWriteStream os(write_fp, writeBuffer, sizeof(writeBuffer));
    //!     3. 用文件流对象初始化write对象
    Writer<FileWriteStream> writer(os);

    // JSON reader parse from the input stream and let writer generate the output.
    //!     4. 调用处理 handle
    CapitalizeFilter<Writer<FileWriteStream> > filter(writer);
    //!     5. 解析到文件
    if (!reader.Parse(is, filter)) {
        fprintf(stderr, "\nError(%u): %s\n", static_cast<unsigned>(reader.GetErrorOffset()), GetParseError_En(reader.GetParseErrorCode()));
    }
    std::cout << writeBuffer <<std::endl;
}

#include "Condense/condense.h"
TEST(stdout_json, condense){
    using namespace rapidjson;

    char readBuffer[65536];

    FILE *read_fp = fopen("/Users/bysouffle/CLionProjects/bys_lib/JsonHandle_tests/test_json.txt", "r");
    //!     1. 初始化读取文件流对象
    FileReadStream is(read_fp, readBuffer, sizeof(readBuffer));
    std::cout << readBuffer << "\n";

    // Prepare JSON writer and output stream.
    char writeBuffer[65536];
    FILE * write_fp = fopen("/Users/bysouffle/CLionProjects/bys_lib/JsonHandle_tests/test_json_std.txt",
                            "w");

    //!     2. 初始化待写入的文件流对象
    FileWriteStream os(write_fp, writeBuffer, sizeof(writeBuffer));

    //!     3. 校验输出
    validation_stdout(is, os);
}

////////////////////////////////////////////////////////////////////////////////
//!!    接收到的云台数据数据解析测试
#include "Archiver/mqtt_struct_ptz.h"
TEST(ptz_opt_data, parse){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":0, "level_value":0,
                                        "vertical_type":0, "vertical_value":0,
                                        "zoom_type":0, "zoom_value":0,
                                        "focus_type":0, "focus_value":0})");
    PtzControl ptz_value;

    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;
//    ptz_value.Print(std::cout);
}

TEST(ptz_opt_data, gen){
    PtzControl ptz_value(1234567890, 0, 0, 0, 0, 0,
                         0, 0, 0);
    JsonWriter writer;
    writer & ptz_value;

    std::cout << writer.GetString() << std::endl;
    std::string json(writer.GetString());
    std::cout << json << std::endl;
}

TEST(ptz_opt_data, parse_left_etc){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":1, "level_value":20,
                                        "vertical_type":1, "vertical_value":0,
                                        "zoom_type":1, "zoom_value":500,
                                        "focus_type":1, "focus_value":0})");
    PtzControl ptz_value;

    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;
    ptz_value.gen_cmd_array();
//    ptz_value.Print(std::cout);
}

TEST(ptz_opt_data, parse_right_etc){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":2, "level_value":20,
                                        "vertical_type":2, "vertical_value":10,
                                        "zoom_type":2, "zoom_value":500,
                                        "focus_type":2, "focus_value":10})");
    PtzControl ptz_value;

    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;
    ptz_value.gen_cmd_array();
//    ptz_value.Print(std::cout);
}

TEST(ptz_opt_data, parse_pos_etc){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":3, "level_value":9,
                                        "vertical_type":3, "vertical_value":10,
                                        "zoom_type":3, "zoom_value":11,
                                        "focus_type":3, "focus_value":12})");
    PtzControl ptz_value;

    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;
    ptz_value.gen_cmd_array();
//    ptz_value.Print(std::cout);
}

TEST(ptz_opt_data, parse_que_etc){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":4, "level_value":0,
                                        "vertical_type":4, "vertical_value":0,
                                        "zoom_type":4, "zoom_value":0,
                                        "focus_type":4, "focus_value":0})");
    PtzControl ptz_value;

    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;
    ptz_value.gen_cmd_array();
//    ptz_value.Print(std::cout);
}

TEST(ptz_opt_data, parse_stop_etc){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":8, "level_value":0,
                                        "vertical_type":8, "vertical_value":0,
                                        "zoom_type":8, "zoom_value":0,
                                        "focus_type":8, "focus_value":0})");
    PtzControl ptz_value;

    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;
    ptz_value.gen_cmd_array();
//    ptz_value.Print(std::cout);
}
////////////////////////////////////////////////////////////////////////////////
//!!    返回云台数据处理结果
TEST(ptz_reply_data, gen_json){
    std::string ptz_cmd_payload(R"({"timestamp":1234567890, "level_type":8, "level_value":0,
                                        "vertical_type":8, "vertical_value":0,
                                        "zoom_type":8, "zoom_value":0,
                                        "focus_type":8, "focus_value":0})");
    PtzControl ptz_value;
    JsonReader reader(ptz_cmd_payload.c_str());
    reader & ptz_value;

//    ptz_value.Print(std::cout);

    PtzControlReply ptz_reply(ptz_value, "1234567890", 0);
    JsonWriter writer;
    writer & ptz_reply;
    std::cout << writer.GetString() << "\n";
}

////////////////////////////////////////////////////////////////////////////////
#include "Archiver/mqtt_struct_switch.h"
//!!    开关处理
TEST(ptz_switch_exec, exec_json){
    std::string switch_json(R"({"timestamp":1234567890, "switch":0})");
    JsonReader reader(switch_json.c_str());
    SwitchControl s;
    reader & s;
    s.Print(std::cout);
}

//!!    开关执行状态返回
TEST(ptz_switch_reply, status_reply){
    std::string switch_json(R"({"timestamp":1234567890, "cid":"1234567890", "status":"成功"})");
    JsonReader reader(switch_json.c_str());
    SwitchControlReply s;
    reader & s;

    s.Print(std::cout);

    SwitchControlReply ss(1234567890, "1234567890", "成功");
    JsonWriter writer;
    writer & ss;
    std::cout << writer.GetString() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
#include "Archiver/mqtt_struct_task.h"
//!!    任务测试
TEST(task_payload, parse){
#if 0
    std::string json(R"({"task_serial":"1234567890",
 "timestamp":123123123,
"task_type":"全面巡检"})");
#endif

    std::string json(R"~({"task_serial": "1234567890",
	"task_type": "全面巡检",
	"timestamp": 1234567890,
	"task": [{
		"task_id": 0,
		"move": {
			"roadmap": "map1",
			"xyz": [1.1, 2.2, 3.3],
			"rpy": [1.1, 2.2, 3.3]
		},
		"ptz": {
			"pos": {
				"level_type": 0,
				"level_value": 36000,
				"vertical_type": 0,
				"vertical_value": 36000,
				"zoom_type": 0,
				"zoom_value": 36000,
				"focus_type": 0,
				"focus_value": 36000
			},
			"action": {
				"reco_type": "表计读数(可识别)",
				"save_type": "红外图片 + 可见光图片",
				"meter_type": "电压表",
				"TH": 1.1,
				"reco_result": 1.1,
				"alarm_level": "正常",
				"max_temp": 1.1,
				"x1_save_url": "/a/b/c"
			}
		}
	}, {
		"task_id": 1,
		"move": {
			"roadmap": "map1",
			"xyz": [2.1, 2.2, 2.3],
			"rpy": [2.1, 2.2, 2.3]
		},
		"ptz": {
			"pos": {
				"level_type": 0,
				"level_value": 36000,
				"vertical_type": 0,
				"vertical_value": 36000,
				"zoom_type": 0,
				"zoom_value": 36000,
				"focus_type": 0,
				"focus_value": 36000
			},
			"action": {
				"reco_type": "红外测温(不可识别)",
				"save_type": "红外图片 + 可见光图片",
				"meter_type": "油位表",
				"TH": 2.2,
				"reco_result": 2.2,
				"alarm_level": "预警",
				"max_temp": 2.2,
				"x1_save_url": "/a/b/c"
			}
		}
	}]
})~");
    JsonReader reader(json.c_str());
    TaskParse s;
    reader & s;

    JsonWriter writer;
    writer&s;
    std::cout << writer.GetString() << "\n";

}

TEST(string_move, std_move){
    std::string tmp;
    {
        std::string move1("abc");
        tmp = std::move(move1);
        std::cout << tmp << "\n";
    }
    {
        std::string move1("def");
        tmp = std::move(move1);
        std::cout << tmp << "\n";
    }
}

class a{
public:
    a():aa("123"){};
    std::string get_aa(){return aa;};
private:
    std::string aa;
};

TEST(a,aa){
    a aaa;
    std::string zz(std::move(aaa.get_aa()));
    std::cout << aaa.get_aa() <<"\n";
}

#include <map>
TEST(map_1,map){
    std::map<int, std::string> map_1;
    map_1[5] = "555";
    map_1[1] = "111";
    map_1[3] = "333";
    map_1[4] = "333";


    std::vector<int> list;
    for(auto &[key, value]:map_1){
        if(value == "333"){
            list.push_back(key);
        }
    }




}

TEST(move_,str_move){
    std::string a = "123";
    auto f = [](const std::string& aa){
        std::string z = std::move(aa);
        std::cout << z ;
    };
    f(a);
    std::cout << a;

}

TEST(arr, append){
    std::array<unsigned char, 8> arr{};

    memcpy(arr.data(),"123",3);

    std::cout << arr.data() << "\n";

}
