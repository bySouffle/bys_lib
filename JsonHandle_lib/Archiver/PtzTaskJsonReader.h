//
// Created by bysou on 2022/1/17.
//

#ifndef BYS_LIB_PTZTASKJSONREADER_H
#define BYS_LIB_PTZTASKJSONREADER_H

#include "archiver.h"
#include <iostream>
#include <utility>
#include <vector>
#include <array>

class PtzTaskJson {
public:
    enum ParamType{
        kLevel = 0,
        kPitch = 1,
        kZoom  = 2,
        kFocus = 3,
    };

    PtzTaskJson() : timestamp(), function(), cmd(),
                    level_angle(), pitch_angle(), zoom_value(), focus_value(),
                    save_url() {}

    PtzTaskJson(uint64_t tm, std::string func, std::string cmd, unsigned short level, unsigned short pitch,
                unsigned short zoom, unsigned short focus, std::string url) : timestamp(tm), function(std::move(func)), cmd(std::move(cmd)),
                                                        level_angle(level), pitch_angle(pitch), zoom_value(zoom),
                                                        focus_value(focus),
                                                        save_url(std::move(url)) {};
    
    ~PtzTaskJson()= default;

    void Print(std::ostream& os);

    //  v2.0 生成给设备的数据
    int gen_device_param(std::array<unsigned char, 2> &device_param, ParamType type){
//        kLevel = 0,
//        kPitch = 1,
//        kZoom  = 2,
//        kFocus = 3,
        switch (type) {
            case kLevel:
            {
                unsigned   temp = level_angle*100;
                device_param[0] = (temp     )& 0xff;
                device_param[1] = (temp >> 8)& 0xff;
                break;
            }
            case kPitch:
            {
                unsigned   temp = pitch_angle*100;
                device_param[0] = (temp     )& 0xff;
                device_param[1] = (temp >> 8)& 0xff;
                break;
            }
            case kZoom:
            {
                unsigned   temp = zoom_value*100;
                device_param[0] = (temp     )& 0xff;
                device_param[1] = (temp >> 8)& 0xff;
                break;
            }
            case kFocus:
            {
                unsigned   temp = focus_value*100;
                device_param[0] = (temp     )& 0xff;
                device_param[1] = (temp >> 8)& 0xff;
                break;
            }
            default:
                return -1;
        }
        return 0;
    }
protected:

    template<typename Archiver>
    friend Archiver& operator&(Archiver& ar, PtzTaskJson& s);

    uint64_t timestamp;
    std::string function;
    std::string cmd;
    unsigned   level_angle;
    unsigned   pitch_angle;
    unsigned   zoom_value;
    unsigned   focus_value;
    std::string save_url;

};

template<typename Archiver>
Archiver &operator&(Archiver &ar, PtzTaskJson &s) {
    ar.StartObject();
    ar.Member("timestamp") & s.timestamp;
    ar.Member("function") & s.function;

    ar.Member("value");
    ar.StartObject();
    ar.Member("cmd") & s.cmd;
    ar.Member("level_angle") & s.level_angle;
    ar.Member("pitch_angle") & s.pitch_angle;
    ar.Member("zoom_value") & s.zoom_value;
    ar.Member("focus_value") & s.focus_value;
    ar.Member("save_url") & s.save_url;
    ar.EndObject();

    return ar.EndObject();
}

void PtzTaskJson::Print(std::ostream &os) {
    os << "start>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    os << "timestamp: " << this->timestamp << "\n";
    os << "function: " << this->function << "\n";
    os << "value: " << "\n";
    os << "cmd: " << this->cmd << "\n";
    os << "level_angle: " << this->level_angle << "\n";
    os << "pitch_angle: " << this->pitch_angle << "\n";
    os << "zoom_value: " << this->zoom_value << "\n";
    os << "focus_value: " << this->focus_value << "\n";
    os << "save_url: " << this->save_url << "\n";
    os << "end<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";


}


#endif //BYS_LIB_PTZTASKJSONREADER_H
