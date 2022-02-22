//
// Created by bys on 2022/2/16.
//

#ifndef BYS_LIB_MQTT_STRUCT_PTZ_H
#define BYS_LIB_MQTT_STRUCT_PTZ_H

#include "archiver.h"
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <numeric>

class TaskPtzData;
class TaskParse;

///////////////////////////////////////////////////////////////////
//! mqtt接收到的云台操作类
class PtzControl {
public:
    //! xxx_type 类型表
    enum ParamType {
        kNoOpt = 0,     //  无操作
        kLeftUp = 1,    //  水平向左|垂直向上|zoom+|focus+
        kRightDown = 2, //  水平向左|垂直向上|zoom+|focus+
        kPosition = 3,  //  定位
        kQuery = 4,     //  查询
        kStop = 8       //  停止
    };

    //! cmdbit_ 参数的统计
    //! 例如 level_type_=0|vertical_type_=1|zoom_type_=3|focus_type_=4 cmdbit = 2 + 4 + 8
    enum CmdBit {
        kCmdBitLevel = 0x1,
        kCmdBitVertical = 0x2,
        kCmdBitZoom = 0x4,
        kCmdBitFocus = 0x8,
    };

    //! 水平指令命令字
    enum LevelCmdBit {
        kLevelLeft = 0x0004,
        kLevelRight = 0x0002,
        kLevelPosition = 0x004B,
        kLevelQuery = 0x0051,
        kLevelStop = 0,
    };

    //! 垂直指令命令字
    enum VerticalCmdBit {
        kVerticalUp = 0x0008,
        kVerticalDown = 0x00010,
        kVerticalPosition = 0x004D,
        kVerticalQuery = 0x0053,
        kVerticalStop = 0,
    };

    //! 缩放指令命令字
    enum ZoomCmdBit {
        kZoomUp = 0x0020,
        kZoomDown = 0x0040,
        kZoomPosition = 0x004F,
        kZoomQuery = 0x0055,
        kZoomStop = 0,
    };

    //! 焦距指令命令字
    enum FocusCmdBit {
        kFocusUp = 0x0080,
        kFocusDown = 0x0100,
        kFocusPosition = 0x006F,
        kFocusQuery = 0x0065,
        kFocusStop = 0,
    };


    PtzControl() : timestamp_(), level_type_(), level_value_(), vertical_type_(), vertical_value_(),
                   zoom_type_(), zoom_value_(), focus_type_(), focus_value_(), cmd_bit_() {};

    PtzControl(uint64_t tm, unsigned level_type, unsigned level_value, unsigned vertical_type, unsigned vertical_value,
               unsigned zoom_type, unsigned zoom_value, unsigned focus_type, unsigned focus_value)
            : timestamp_(tm), level_type_(level_type), level_value_(level_value),
              vertical_type_(vertical_type), vertical_value_(vertical_value),
              zoom_type_(zoom_type), zoom_value_(zoom_value), focus_type_(focus_type), focus_value_(focus_value),
              cmd_bit_() {};

    ~PtzControl() = default;

    void Print(std::ostream &os)=delete;

    //! 计算操作bit
    //! \return 操作bit值
    int calc_cmd_bit() {
        if (cmd_bit_ == 0) {
            cmd_bit_ = level_type_ > 0 ? cmd_bit_ + kCmdBitLevel : cmd_bit_;
            cmd_bit_ = vertical_type_ > 0 ? cmd_bit_ + kCmdBitVertical : cmd_bit_;
            cmd_bit_ = zoom_type_ > 0 ? cmd_bit_ + kCmdBitZoom : cmd_bit_;
            cmd_bit_ = focus_type_ > 0 ? cmd_bit_ + kCmdBitFocus : cmd_bit_;
            return cmd_bit_;
        } else {
            return cmd_bit_;
        }
    }

    //! 生成云台命令字节参数和操作中文
    //! 需要先有操作bit
    //! \return
    int gen_cmd_array() {
        calc_cmd_bit();
        //  判断操作bit是否有水平操作
        if ((cmd_bit_ & kCmdBitLevel) == kCmdBitLevel) {
            switch (level_type_) {
                case kNoOpt:    //  无操作
                    level_type_zh.append("无操作");
                    break;
                case kLeftUp:   //  水平向左
                    level_type_zh.append("水平向左");
                    level_cmd_[0] = 0xff;
                    level_cmd_[1] = 0x01;
                    level_cmd_[2] = (kLevelLeft >> 8) & 0xff;
                    level_cmd_[3] = (kLevelLeft) & 0xff;
                    level_cmd_[4] = level_value_;      //  0-64
                    level_cmd_[5] = 0;
                    level_cmd_[6] = std::accumulate(level_cmd_.begin() + 1, level_cmd_.end() - 1, 0);
                    break;
                case kRightDown://  水平向右
                    level_type_zh.append("水平向右");
                    level_cmd_[0] = 0xff;
                    level_cmd_[1] = 0x01;
                    level_cmd_[2] = (kLevelRight >> 8) & 0xff;
                    level_cmd_[3] = (kLevelRight) & 0xff;
                    level_cmd_[4] = level_value_;       //  0-64
                    level_cmd_[5] = 0;
                    level_cmd_[6] = std::accumulate(level_cmd_.begin() + 1, level_cmd_.end() - 1, 0);
                    break;
                case kQuery:    //  水平查询
                    level_type_zh.append("水平查询");
                    level_cmd_[0] = 0xff;
                    level_cmd_[1] = 0x01;
                    level_cmd_[2] = (kLevelQuery >> 8) & 0xff;
                    level_cmd_[3] = (kLevelQuery) & 0xff;
                    level_cmd_[4] = 0;
                    level_cmd_[5] = 0;
                    level_cmd_[6] = std::accumulate(level_cmd_.begin() + 1, level_cmd_.end() - 1, 0);
                    break;
                case kPosition: //  水平定位
                    level_type_zh.append("水平定位");
                    level_cmd_[0] = 0xff;
                    level_cmd_[1] = 0x01;
                    level_cmd_[2] = (kLevelPosition >> 8) & 0xff;
                    level_cmd_[3] = (kLevelPosition) & 0xff;
                    level_cmd_[4] = (level_value_ >> 8) & 0xff;
                    level_cmd_[5] = (level_value_) & 0xff;
                    level_cmd_[6] = std::accumulate(level_cmd_.begin() + 1, level_cmd_.end() - 1, 0);
                    break;
                case kStop:     //  水平停止操作
                    level_type_zh.append("水平停止操作");
                    level_cmd_[0] = 0xff;
                    level_cmd_[1] = 0x01;
                    level_cmd_[2] = (kLevelStop >> 8) & 0xff;
                    level_cmd_[3] = (kLevelStop) & 0xff;
                    level_cmd_[4] = 0;
                    level_cmd_[5] = 0;
                    level_cmd_[6] = std::accumulate(level_cmd_.begin() + 1, level_cmd_.end() - 1, 0);
                    break;
                default:        //  未知操作
                    level_type_zh.append("未知操作");
                    break;
            }
        }
        if ((cmd_bit_ & kCmdBitVertical) == kCmdBitVertical) {
            switch (vertical_type_) {
                case kNoOpt:    //  无操作
                {
                    vertical_type_zh.append("无操作");
                    break;
                }
                case kLeftUp:   //  垂直向上
                {
                    vertical_type_zh.append("垂直向上");
                    vertical_cmd_[0] = 0xff;
                    vertical_cmd_[1] = 0x01;
                    vertical_cmd_[2] = (kVerticalUp >> 8) & 0xff;
                    vertical_cmd_[3] = (kVerticalUp) & 0xff;
                    vertical_cmd_[4] = 0;
                    vertical_cmd_[5] = vertical_value_; //  0-64
                    vertical_cmd_[6] = std::accumulate(vertical_cmd_.begin() + 1, vertical_cmd_.end() - 1, 0);
                    break;
                }
                case kRightDown://  垂直向下
                {
                    vertical_type_zh.append("垂直向下");
                    vertical_cmd_[0] = 0xff;
                    vertical_cmd_[1] = 0x01;
                    vertical_cmd_[2] = (kVerticalDown >> 8) & 0xff;
                    vertical_cmd_[3] = (kVerticalDown) & 0xff;
                    vertical_cmd_[4] = 0;
                    vertical_cmd_[5] = vertical_value_; //  0-64
                    vertical_cmd_[6] = std::accumulate(vertical_cmd_.begin() + 1, vertical_cmd_.end() - 1, 0);
                    break;
                }
                case kQuery:    //  查询
                {
                    vertical_type_zh.append("垂直查询");
                    vertical_cmd_[0] = 0xff;
                    vertical_cmd_[1] = 0x01;
                    vertical_cmd_[2] = (kVerticalQuery >> 8) & 0xff;
                    vertical_cmd_[3] = (kVerticalQuery) & 0xff;
                    vertical_cmd_[4] = 0;
                    vertical_cmd_[5] = 0;
                    vertical_cmd_[6] = std::accumulate(vertical_cmd_.begin() + 1, vertical_cmd_.end() - 1, 0);
                    break;
                }
                case kPosition:     //  垂直定位
                {
                    vertical_type_zh.append("垂直定位");
                    vertical_cmd_[0] = 0xff;
                    vertical_cmd_[1] = 0x01;
                    vertical_cmd_[2] = (kVerticalPosition >> 8) & 0xff;
                    vertical_cmd_[3] = (kVerticalPosition) & 0xff;
                    vertical_cmd_[4] = (vertical_value_ >> 8) & 0xff;
                    vertical_cmd_[5] = (vertical_value_) & 0xff;
                    vertical_cmd_[6] = std::accumulate(vertical_cmd_.begin() + 1, vertical_cmd_.end() - 1, 0);
                    break;
                }
                case kStop:     //  垂直停止操作
                {
                    vertical_type_zh.append("垂直停止操作");
                    vertical_cmd_[0] = 0xff;
                    vertical_cmd_[1] = 0x01;
                    vertical_cmd_[2] = (kVerticalStop >> 8) & 0xff;
                    vertical_cmd_[3] = (kVerticalStop) & 0xff;
                    vertical_cmd_[4] = 0;
                    vertical_cmd_[5] = 0;
                    vertical_cmd_[6] = std::accumulate(vertical_cmd_.begin() + 1, vertical_cmd_.end() - 1, 0);
                    break;
                }
                default:        //  未知操作
                {
                    vertical_type_zh.append("未知操作");
                    break;
                }
            }
        }
        if ((cmd_bit_ & kCmdBitZoom) == kCmdBitZoom) {
            switch (zoom_type_) {
                case kNoOpt:    //  无操作
                {
                    zoom_type_zh.append("无操作");
                    break;
                }
                case kLeftUp:   //  缩放+
                {
                    zoom_type_zh.append("缩放+");
                    zoom_cmd_[0] = 0xff;
                    zoom_cmd_[1] = 0x01;
                    zoom_cmd_[2] = (kZoomUp >> 8) & 0xff;
                    zoom_cmd_[3] = (kZoomUp) & 0xff;
                    zoom_cmd_[4] = 0;
                    zoom_cmd_[5] = 0;
                    zoom_cmd_[6] = std::accumulate(zoom_cmd_.begin() + 1, zoom_cmd_.end() - 1, 0);
                    break;
                }
                case kRightDown://  缩放-
                {
                    zoom_type_zh.append("缩放-");
                    zoom_cmd_[0] = 0xff;
                    zoom_cmd_[1] = 0x01;
                    zoom_cmd_[2] = (kZoomDown >> 8) & 0xff;
                    zoom_cmd_[3] = (kZoomDown) & 0xff;
                    zoom_cmd_[4] = 0;
                    zoom_cmd_[5] = 0;
                    zoom_cmd_[6] = std::accumulate(zoom_cmd_.begin() + 1, zoom_cmd_.end() - 1, 0);
                    break;
                }
                case kQuery:    //  缩放查询
                {
                    zoom_type_zh.append("缩放查询");
                    zoom_cmd_[0] = 0xff;
                    zoom_cmd_[1] = 0x01;
                    zoom_cmd_[2] = (kZoomQuery >> 8) & 0xff;
                    zoom_cmd_[3] = (kZoomQuery) & 0xff;
                    zoom_cmd_[4] = 0;
                    zoom_cmd_[5] = 0;
                    zoom_cmd_[6] = std::accumulate(zoom_cmd_.begin() + 1, zoom_cmd_.end() - 1, 0);
                    break;
                }
                case kPosition:     //  缩放定位
                {
                    zoom_type_zh.append("缩放定位");
                    zoom_cmd_[0] = 0xff;
                    zoom_cmd_[1] = 0x01;
                    zoom_cmd_[2] = (kZoomPosition >> 8) & 0xff;
                    zoom_cmd_[3] = (kZoomPosition) & 0xff;
                    zoom_cmd_[4] = (zoom_value_ >> 8) & 0xff;
                    zoom_cmd_[5] = (zoom_value_) & 0xff;
                    zoom_cmd_[6] = std::accumulate(zoom_cmd_.begin() + 1, zoom_cmd_.end() - 1, 0);
                    break;
                }
                case kStop:     //  缩放停止操作
                {
                    zoom_type_zh.append("缩放停止操作");
                    zoom_cmd_[0] = 0xff;
                    zoom_cmd_[1] = 0x01;
                    zoom_cmd_[2] = (kZoomStop >> 8) & 0xff;
                    zoom_cmd_[3] = (kZoomStop) & 0xff;
                    zoom_cmd_[4] = 0;
                    zoom_cmd_[5] = 0;
                    zoom_cmd_[6] = std::accumulate(zoom_cmd_.begin() + 1, zoom_cmd_.end() - 1, 0);
                    break;
                }
                default:        //  未知操作
                {
                    zoom_type_zh.append("未知操作");
                    break;
                }
            }
        }
        if ((cmd_bit_ & kCmdBitFocus) == kCmdBitFocus) {
            switch (focus_type_) {
                case kNoOpt:    //  无操作
                {
                    focus_type_zh.append("无操作");
                    break;
                }
                case kLeftUp:   //  焦距+
                {
                    focus_type_zh.append("焦距+");
                    focus_cmd_[0] = 0xff;
                    focus_cmd_[1] = 0x01;
                    focus_cmd_[2] = (kFocusUp >> 8) & 0xff;
                    focus_cmd_[3] = (kFocusUp) & 0xff;
                    focus_cmd_[4] = 0;
                    focus_cmd_[5] = 0;
                    focus_cmd_[6] = std::accumulate(focus_cmd_.begin() + 1, focus_cmd_.end() - 1, 0);
                    break;
                }
                case kRightDown://  焦距-
                {
                    focus_type_zh.append("焦距-");
                    focus_cmd_[0] = 0xff;
                    focus_cmd_[1] = 0x01;
                    focus_cmd_[2] = (kFocusDown >> 8) & 0xff;
                    focus_cmd_[3] = (kFocusDown) & 0xff;
                    focus_cmd_[4] = 0;
                    focus_cmd_[5] = 0;
                    focus_cmd_[6] = std::accumulate(focus_cmd_.begin() + 1, focus_cmd_.end() - 1, 0);
                    break;
                }
                case kQuery:    //  焦距查询
                {
                    focus_type_zh.append("焦距查询");
                    focus_cmd_[0] = 0xff;
                    focus_cmd_[1] = 0x01;
                    focus_cmd_[2] = (kFocusQuery >> 8) & 0xff;
                    focus_cmd_[3] = (kFocusQuery) & 0xff;
                    focus_cmd_[4] = 0;
                    focus_cmd_[5] = 0;
                    focus_cmd_[6] = std::accumulate(focus_cmd_.begin() + 1, focus_cmd_.end() - 1, 0);
                    break;
                }
                case kPosition: {
                    focus_type_zh.append("焦距定位");
                    focus_cmd_[0] = 0xff;
                    focus_cmd_[1] = 0x01;
                    focus_cmd_[2] = (kFocusPosition >> 8) & 0xff;
                    focus_cmd_[3] = (kFocusPosition) & 0xff;
                    focus_cmd_[4] = (focus_value_ >> 8) & 0xff;
                    focus_cmd_[5] = (focus_value_) & 0xff;
                    focus_cmd_[6] = std::accumulate(focus_cmd_.begin() + 1, focus_cmd_.end() - 1, 0);
                    break;
                }
                case kStop:     //  焦距停止操作
                {
                    focus_type_zh.append("焦距停止操作");
                    focus_cmd_[0] = 0xff;
                    focus_cmd_[1] = 0x01;
                    focus_cmd_[2] = (kFocusStop >> 8) & 0xff;
                    focus_cmd_[3] = (kFocusStop) & 0xff;
                    focus_cmd_[4] = 0;
                    focus_cmd_[5] = 0;
                    focus_cmd_[6] = std::accumulate(focus_cmd_.begin() + 1, focus_cmd_.end() - 1, 0);
                    break;
                }
                default:        //  未知操作
                {
                    focus_type_zh.append("未知操作");
                    break;
                }
            }
        }
        return 0;
    }

public:
    uint64_t getTimestamp() const {
        return timestamp_;
    }

    unsigned int getLevelType() const {
        return level_type_;
    }

    unsigned int getVerticalType() const {
        return vertical_type_;
    }

    unsigned int getZoomType() const {
        return zoom_type_;
    }

    unsigned int getFocusType() const {
        return focus_type_;
    }

protected:

    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, PtzControl &s);

    uint64_t timestamp_;

    unsigned level_type_;
    unsigned level_value_;
    std::string level_type_zh;

    unsigned vertical_type_;
    unsigned vertical_value_;
    std::string vertical_type_zh;

    unsigned zoom_type_;
    unsigned zoom_value_;
    std::string zoom_type_zh;

    unsigned focus_type_;
    unsigned focus_value_;
    std::string focus_type_zh;

    unsigned char cmd_bit_;

    std::array<unsigned char, 7> level_cmd_{};
    std::array<unsigned char, 7> vertical_cmd_{};
    std::array<unsigned char, 7> zoom_cmd_{};
    std::array<unsigned char, 7> focus_cmd_{};

    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, TaskParse &s);

};

template<typename Archiver>
Archiver &operator&(Archiver &ar, PtzControl &s) {
//    ar.StartObject();
    ar.Member("timestamp") & s.timestamp_;
    ar.Member("level_type") & s.level_type_;
    ar.Member("level_value") & s.level_value_;

    ar.Member("vertical_type") & s.vertical_type_;
    ar.Member("vertical_value") & s.vertical_value_;

    ar.Member("zoom_type") & s.zoom_type_;
    ar.Member("zoom_value") & s.zoom_value_;

    ar.Member("focus_type") & s.focus_type_;
    ar.Member("focus_value") & s.focus_value_;
//    ar.EndObject();
//    return ar.EndObject();
    return ar;
}

#if 0
void PtzControl::Print(std::ostream &os) {
    os << "start>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    os << "timestamp: " << this->timestamp_ << "\n";
    os << "水平操作: " << this->level_type_zh << "\t" << std::dec << this->level_value_ << "\tcmd_bit: ";
    for (auto &it: level_cmd_) {
        os << std::hex << "[0x" << (0xff & it) << "] ";
    }
    os << "\n";
    os << "垂直操作: " << this->vertical_type_zh << "\t" << std::dec << this->vertical_value_ << "\tcmd_bit: ";
    for (auto &it: vertical_cmd_) {
        os << std::hex << "[0x" << (0xff & it) << "] ";
    }
    os << "\n";
    os << "缩放操作: " << this->zoom_type_zh << "\t" << std::dec << this->zoom_value_ << "\tcmd_bit: ";
    for (auto &it: zoom_cmd_) {
        os << std::hex << "[0x" << (0xff & it) << "] ";
    }
    os << "\n";
    os << "焦距操作: " << this->focus_type_zh << "\t" << std::dec << this->focus_value_ << "\tcmd_bit: ";
    for (auto &it: focus_cmd_) {
        os << std::hex << "[0x" << (0xff & it) << "] ";
    }
    os << "\n";
    os << "end<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";


}
#endif
///////////////////////////////////////////////////////////////////
//! mqtt返回的数据类
#if 0
class PtzControlReply {
public:
    PtzControlReply(PtzControl &ptz_control, std::string cid, int status)
            : timestamp_(ptz_control.getTimestamp()), cid_(std::move(cid)), level_type_(ptz_control.getLevelType()),
            level_value_(), vertical_type_(ptz_control.getVerticalType()), vertical_value_(),
            zoom_type_(ptz_control.getZoomType()), zoom_value_(),
            focus_type_(ptz_control.getFocusType()), focus_value_(), status_(status) {};

    PtzControlReply() : timestamp_(), cid_(), level_type_(), level_value_(),
                        vertical_type_(), vertical_value_(), zoom_type_(), zoom_value_(),
                        focus_type_(), focus_value_(), status_() {};

    PtzControlReply(uint64_t timestamp, std::string cid, unsigned int levelType,
                    unsigned int levelValue, unsigned int verticalType, unsigned int verticalValue,
                    unsigned int zoomType, unsigned int zoomValue, unsigned int focusType,
                    unsigned int focusValue, int status)
            : timestamp_(timestamp), cid_(std::move(cid)), level_type_(levelType), level_value_(levelValue),
              vertical_type_(verticalType), vertical_value_(verticalValue), zoom_type_(zoomType),
              zoom_value_(zoomValue),
              focus_type_(focusType), focus_value_(focusValue), status_(status) {};


protected:
    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, PtzControlReply &s);

    uint64_t timestamp_;

    std::string cid_;

    unsigned level_type_;
    unsigned level_value_;
    std::string level_type_zh;

    unsigned vertical_type_;
    unsigned vertical_value_;
    std::string vertical_type_zh;

    unsigned zoom_type_;
    unsigned zoom_value_;
    std::string zoom_type_zh;

    unsigned focus_type_;
    unsigned focus_value_;
    std::string focus_type_zh;
    int status_;
};

template<typename Archiver>
Archiver &operator&(Archiver &ar, PtzControlReply &s) {
    ar.StartObject();
    ar.Member("timestamp") & s.timestamp_;
    ar.Member("cid") & s.cid_;

    ar.Member("level_type") & s.level_type_;
    ar.Member("level_value") & s.level_value_;

    ar.Member("vertical_type") & s.vertical_type_;
    ar.Member("vertical_value") & s.vertical_value_;

    ar.Member("zoom_type") & s.zoom_type_;
    ar.Member("zoom_value") & s.zoom_value_;

    ar.Member("focus_type") & s.focus_type_;
    ar.Member("focus_value") & s.focus_value_;

    ar.Member("status") & s.status_;

    return ar.EndObject();
}
#endif

class PtzControlReply : public PtzControl {
public:
    PtzControlReply() : cid_(), status_() {};
    PtzControlReply(const PtzControl& control_data, std::string cid, int status) : PtzControl(control_data),
    cid_(std::move(cid)), status_(status) {};
    PtzControlReply(uint64_t tm, unsigned level_type, unsigned level_value,
                    unsigned vertical_type, unsigned vertical_value,
                    unsigned zoom_type, unsigned zoom_value, unsigned focus_type, unsigned focus_value, std::string cid,
                    int status)
            : PtzControl(tm, level_type, level_value, vertical_type, vertical_value, zoom_type, zoom_value,
                         focus_type, focus_value), cid_(std::move(cid)), status_(status) {};
private:
    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, PtzControlReply &s);

    std::string cid_;
    int status_;

};

template<typename Archiver>
Archiver &operator&(Archiver &ar, PtzControlReply &s) {
    ar & static_cast<PtzControl &>(s);
    ar.Member("cid") & s.cid_;
    ar.Member("status") & s.status_;
    return ar;
}


#endif //BYS_LIB_MQTT_STRUCT_PTZ_H
