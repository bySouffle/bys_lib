//
// Created by bys on 2022/2/17.
//

#include "mqtt_struct_task.h"

#include <utility>

TaskItemData::TaskItemData(): TaskCarData(), TaskPtzData() {

}

TaskItemData::TaskItemData(TaskCarData &task_car_data, TaskPtzData &task_ptz_data, unsigned task_id)
: TaskCarData(task_car_data), TaskPtzData(task_ptz_data), task_id_(task_id)
{

}


TaskCarData::TaskCarData() : roadmap_(), xyz_(), rpy_() {

}

TaskCarData::TaskCarData(std::string roadmap, std::array<double, 3> xyz, std::array<double, 3> rpy)
        : roadmap_(std::move(roadmap)), xyz_(xyz), rpy_(rpy) {}

TaskPtzData::TaskPtzData() : ptz_move_data_(), ptz_action_data_(){}

TaskPtzActionData::TaskPtzActionData() : recognition_type(), save_type(), meter_type(),
        threshold(), recognition_result(), max_temperature(), x1_save_url()
{

}

TaskParse::TaskParse() : task_serial_(), task_type_(), timestamp_() {

}

TaskParse::TaskParse(std::string taskSerial, std::string taskType, uint64_t timestamp)
        : task_serial_(std::move(taskSerial)), task_type_(std::move(taskType)), timestamp_(timestamp) {}
