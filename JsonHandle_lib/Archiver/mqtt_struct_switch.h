//
// Created by bys on 2022/2/17.
//

#ifndef BYS_LIB_MQTT_STRUCT_SWITCH_H
#define BYS_LIB_MQTT_STRUCT_SWITCH_H

#include "archiver.h"
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <numeric>

/////////////////////////////////////////////////////////////////
class SwitchControl {
public:
    //! switch 状态表
    enum SwitchStatus {
        kSwitchClose = 0,      //  关
        kSwitchOpen = 1,      //  开
    };
    SwitchControl()
            : timestamp_(), switch_() {};

    SwitchControl(uint64_t timestamp, int s_switch)
            : timestamp_(timestamp), switch_(s_switch) {};

    ~SwitchControl() = default;

    void Print(std::ostream &os);

protected:

    uint64_t timestamp_;
    int switch_;

    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, SwitchControl &s);

};

template<typename Archiver>
Archiver &operator&(Archiver &ar, SwitchControl &s) {
    ar.StartObject();
    ar.Member("timestamp") & s.timestamp_;
    ar.Member("switch") & s.switch_;

    return ar.EndObject();
}

void SwitchControl::Print(std::ostream &os) {
    os << "switch>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    os << "timestamp: " << this->timestamp_ << "\n";
    os << "开关状态: " << ((switch_==1)?"开":"关") << "\n";
    os << "switch end<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
}

/////////////////////////////////////////////////////////////////
class SwitchControlReply {
public:
    SwitchControlReply()
            : timestamp_(), status_() {};

    SwitchControlReply(uint64_t timestamp, std::string cid, std::string status)
            : timestamp_(timestamp), cid_(std::move(cid)), status_(std::move(status)) {};

    ~SwitchControlReply() = default;

    void Print(std::ostream &os);

protected:

    uint64_t timestamp_;
    std::string cid_;
    std::string status_;

    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, SwitchControlReply &s);

};

template<typename Archiver>
Archiver &operator&(Archiver &ar, SwitchControlReply &s) {
    ar.StartObject();
    ar.Member("timestamp") & s.timestamp_;
    ar.Member("cid") & s.cid_;
    ar.Member("status") & s.status_;

    return ar.EndObject();
}

void SwitchControlReply::Print(std::ostream &os) {
    os << "switchReply>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    os << "timestamp: " << this->timestamp_ << "\n";
    os << "cid: " << this->cid_ << "\n";
    os << "状态: " << this->status_ << "\n";
    os << "switchReply end<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
}


#endif //BYS_LIB_MQTT_STRUCT_SWITCH_H
