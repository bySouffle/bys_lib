//
// Created by bysou on 2021/12/25.
//

#include "gtest/gtest.h"
#include "archiver/archiver.h"
#include "archiver/archiver_obj.h"
#include "archiver/PtzTaskJsonReader.h"


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

    SysInfo(const std::string& version,const std::string& release,const std::vector<int>&disk_info,
            const std::string& mac);
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

SysInfo::SysInfo(const std::string &version, const std::string &release, const std::vector<int> &disk_info,
                 const std::string &mac):linux_version(version), release(release),disk_info(disk_info),
                                         mac(mac) {

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

}
////////////////////////////////////////////////////////////////////////////////

TEST(ptz, data_write){
    int timestamp = 202101172047;
    std::string function("ptz");
    std::string cmd("auto_exec");
    int level_angle = 360;
    int pitch_angle = 90;
    int zoom_value = 100;
    int focus_value = 10;
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
}