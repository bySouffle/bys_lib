//
// Created by bys on 2022/2/17.
//

#ifndef BYS_LIB_MQTT_STRUCT_TASK_H
#define BYS_LIB_MQTT_STRUCT_TASK_H

#include "archiver.h"
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include "mqtt_struct_ptz.h"
////!!    车坐标
//typedef struct CarCoordInfo{
//    std::string roadmap;
//    std::array<double,3> xyz;
//    std::array<double,3> rpy;
//}CarCoordInfo_t;
//
//
//
////!!    云台控制数据定义
////  class PtzControl
//typedef struct PtzTaskInfo{
//    unsigned level_type;
//    unsigned level_value;
//
//    unsigned vertical_type;
//    unsigned vertical_value;
//
//    unsigned zoom_type;
//    unsigned zoom_value;
//
//    unsigned focus_type;
//    unsigned focus_value;
//}PtzTaskInfo_t;

////!!    云台动作数据定义
//typedef struct PtzActionInfo{
//    std::string recognition_type;   //  识别类型
//    std::string save_type;
//    std::string meter_type;
//    int threshold;
//    int recognition_result;
//    int max_temperature;
//    std::string x1_save_url;
//}PtzActionInfo_t;



//!!    任务数据    --云台动作
class TaskPtzActionData{
public:
    TaskPtzActionData();
protected:
    std::string recognition_type;   //  识别类型
    std::string save_type;
    std::string meter_type;
    double threshold;
    double recognition_result;
    double max_temperature;
    std::string x1_save_url;
    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, TaskParse& s);
};

//!!    任务数据    --云台
class TaskPtzData{
public:
    TaskPtzData();
protected:
    PtzControl ptz_move_data_;
    TaskPtzActionData ptz_action_data_;
    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, TaskParse& s);
};

//!!    任务数据    --车
class TaskCarData{
public:
    TaskCarData();
    TaskCarData(std::string roadmap, std::array<double, 3> xyz, std::array<double, 3> rpy);

protected:
    std::string roadmap_;
    std::array<double,3> xyz_;
    std::array<double,3> rpy_;

    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, TaskParse& s);
};

//!!    任务数据    --task
class TaskItemData : public TaskCarData,TaskPtzData{
public:
    TaskItemData();
    TaskItemData(TaskCarData& task_car_data, TaskPtzData& task_ptz_data, unsigned task_id);
//    TaskItemData(std::string & roadmap, std::array<double,3>& xyz, std::array<double,3>& rpy );

protected:
//    TaskCarData car_data;
//    TaskPtzData ptz_data;
    unsigned task_id_;
    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, TaskParse& s);
};

//!!    任务解析
class TaskParse {
public:
    TaskParse();

    TaskParse(std::string taskSerial, std::string taskType, uint64_t timestamp);

protected:
    std::string task_serial_;
    std::string task_type_;
    uint64_t timestamp_;
    std::unordered_map<unsigned, TaskItemData> task_items_;
//
    size_t task_cnt_;

    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, TaskParse& s);

};

template<typename Archiver>
Archiver &operator&(Archiver &ar, TaskParse &s) {
    ar.StartObject();   //  {}
    ar.Member("task_serial") & s.task_serial_;
    ar.Member("timestamp") & s.timestamp_;
    ar.Member("task_type") & s.task_type_;
//    return ar.EndObject();

#if 01
    ar.Member("task");
    s.task_cnt_ = s.task_items_.size();
    //  "task":[]
    ar.StartArray(&s.task_cnt_ );
    for (size_t i = 0; i < s.task_cnt_; ++i) {
        //  "task":[{}]
        ar.StartObject();
        {
            TaskItemData temp_item; //  任务中间变量
            if(ar.IsWriter) temp_item = std::move(s.task_items_.at(i));

//            unsigned id = 0;
            ar.Member("task_id") & temp_item.task_id_;
            {
                //  "move":{}
                ar.Member("move");
                ar.StartObject();
                ar.Member("roadmap") & temp_item.roadmap_ ;
                {
                    size_t xyz_cnt = temp_item.xyz_.size();
                    //  "xyz":[]
                    ar.Member("xyz");
                    ar.StartArray(&xyz_cnt);
                    for (size_t j = 0; j < xyz_cnt; ++j) {
                        ar&temp_item.xyz_[j];
                    }
                    ar.EndArray();  //  "xyz":[]  end
                    size_t rpy_cnt = temp_item.rpy_.size();
                    //  "rpy":[]
                    ar.Member("rpy");
                    ar.StartArray(&rpy_cnt);
                    for (size_t j = 0; j < rpy_cnt; ++j) {
                        ar&temp_item.rpy_[j];
                    }
                    ar.EndArray();  //  "rpy"[] end
                }
                ar.EndObject(); //  "move":{}   end
            }
            {
                //  "ptz":{}
                ar.Member("ptz");
                ar.StartObject();
                {
                    //  "pos":{}
                    ar.Member("pos");
                    ar.StartObject();
                    ar.Member("level_type") & temp_item.ptz_move_data_.level_type_;
                    ar.Member("level_value") & temp_item.ptz_move_data_.level_value_;

                    ar.Member("vertical_type") & temp_item.ptz_move_data_.vertical_type_;
                    ar.Member("vertical_value") & temp_item.ptz_move_data_.vertical_value_;

                    ar.Member("zoom_type") & temp_item.ptz_move_data_.zoom_type_;
                    ar.Member("zoom_value") & temp_item.ptz_move_data_.zoom_value_;

                    ar.Member("focus_type") & temp_item.ptz_move_data_.focus_type_;
                    ar.Member("focus_value") & temp_item.ptz_move_data_.focus_value_;
                    ar.EndObject(); //  "pos":{}    end
                }
                {
                    //  "action":{}
                    ar.Member("action");
                    ar.StartObject();
                    ar.Member("reco_type") & temp_item.ptz_action_data_.recognition_type;
                    ar.Member("save_type") & temp_item.ptz_action_data_.save_type;

                    ar.Member("meter_type") & temp_item.ptz_action_data_.meter_type;
                    ar.Member("TH") & temp_item.ptz_action_data_.threshold;

                    ar.Member("reco_result") & temp_item.ptz_action_data_.recognition_result;

                    ar.Member("max_temp") & temp_item.ptz_action_data_.max_temperature;
                    ar.Member("x1_save_url") & temp_item.ptz_action_data_.x1_save_url;
                    ar.EndObject(); //  "action":{}    end
                }
                ar.EndObject(); //  "ptz":{}   end
            }

            if(ar.IsReader) s.task_items_[temp_item.task_id_] = std::move(temp_item);   //  存入

        }
        ar.EndObject(); //  "task":[{}] end

    }
    ar.EndArray();  //  "task":[{}] end
    return ar.EndObject();   //  {}  end
#endif
}



#endif //BYS_LIB_MQTT_STRUCT_TASK_H
