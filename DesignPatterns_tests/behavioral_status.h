//
// Created by bysouffle on 2021/11/14.
//

#ifndef BYS_LIB_BEHAVIORAL_STATUS_H
#define BYS_LIB_BEHAVIORAL_STATUS_H

namespace Status_mode_all_status{
    class CandyMaMachineStatus{
    public:
        CandyMaMachineStatus();
        virtual ~CandyMaMachineStatus();

        virtual void insert_coin() = 0;
        virtual void turn_crank() = 0;
        virtual void cancel_insert() = 0;
        virtual void display_result() = 0;
    };

    //  1. 未投币
    //  2. 投币
    //  3. 出售
    //  4.
}


#endif //BYS_LIB_BEHAVIORAL_STATUS_H
