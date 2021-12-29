//
// Created by bys on 2021/12/8.
//

#include "ThreadOutSysInfo.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

//！线程启动
void ThreadOutSysInfo::start() {
    t_ = new std::thread( &ThreadOutSysInfo::routine_task, this, this);

}

/*
{
	"os": {
		"sysname": "Linux",             //  系统类型
		"nodename": "siasun-NUC8i7BEH", //  主机用户用
		"release": "5.4.0-89-generic",  //  linux版本号
		"version": "#100-Ubuntu SMP Fri Sep 24 14:50:10 UTC 2021",  //  系统版本号
		"arch": "x86_64"                //  系统硬件架构
	},
	"mac": "xx-xx-7A-6B-E8-18",     //  网卡mac地址
	"cpu_usage": 0.5031,            //  CPU使用率
	"mem_info":  [16262784, 12159080, 8004832, 0.4922],               // MB  [总内存，剩余内存, 已用内存，使用率]
	"disk_info": [109,      85,       24,      0.22018348623853212]   // GB  [总硬盘，剩余容量, 已用容量，使用率]
}

 * */

//! 线程入口
//! \param param
void ThreadOutSysInfo::routine_task(void *param) {
    auto* p_this = static_cast<ThreadOutSysInfo*>(param);

    StringBuffer s;

    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(5));

        Writer<StringBuffer> writer(s);
        writer.StartObject();

        //  1. 获取os信息
        p_this->s_->GetSysInfo(u);
#if 0
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
            writer.String(u->sysname);
            writer.Key("nodename");
            writer.String(u->nodename);
            writer.Key("release");
            writer.String(u->release);
            writer.Key("version");
            writer.String(u->version);
            writer.Key("arch");
            writer.String(u->machine);
//            writer.Key("domainname");
//            writer.String(u->domainname);
            writer.EndObject();
        }

        //  2. 获取MAC
        mac_.append( p_this->s_->GetSysMac() ) ;
#if 0
        printf(">>>>>>>>>>>>>> mac info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [mac %s]\n", mac_.data());
        printf("<<<<<<<<<<< mac info  end <<<<<<<<<<<<<<<\n" );
#endif
        writer.Key("mac");
        writer.String(mac_.data());

        //  3. 获取CPU
        cpu_usage_ = p_this->s_->GetTotalCpuPercent();
#if 0
        printf(">>>>>>>>>>>>>> cpu info >>>>>>>>>>>>>>>>>\n" );
        printf("1 [cpu %.4f]\n", cpu_usage_);
        printf("<<<<<<<<<<< cpu info  end <<<<<<<<<<<<<<<\n" );
#endif
        writer.Key("cpu_usage");
        writer.Double(cpu_usage_);

        //  4. 获取内存
        p_this->s_->GetMemInfo(*mem_info_);
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
            writer.Int(mem_info_->total);
            writer.Int(mem_info_->free);
            int mem_used = mem_info_->total - mem_info_->free;
            writer.Int(mem_used);
            writer.Double( mem_used/(mem_info_->total*1.0000) );
            writer.EndArray();
        }

        //  5. 获取硬盘信息
        p_this->s_->GetDiskPercent(disk_info_);
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
            writer.Int(disk_info_->total);
            writer.Int(disk_info_->free);
            writer.Int(disk_info_->used);
            writer.Double(disk_info_->Usage);
            writer.EndArray();
        }
        writer.EndObject();
        printf("%s\n", s.GetString());

        //  public

        {
            //  clear
            mac_.clear();
            msg.clear();
            memset( disk_info_, 0, sizeof( DiskInfo_t ) );
            memset( mem_info_ , 0, sizeof( MemInfo_t  ) );
            memset( u         , 0, sizeof( struct utsname  ) );
            s.Clear();

            printf("after clear %s\n", s.GetString());
        }

        if(exit_flag_ == 1){
            break;
        }
    }

}


ThreadOutSysInfo::ThreadOutSysInfo():exit_flag_(0), msg(), mac_(), cpu_usage_(0), u(new struct utsname()),
                                     disk_info_(new DiskInfo_t()), mem_info_(new MemInfo_t),
                                     s_(new SystemMonitor) {

}

ThreadOutSysInfo::~ThreadOutSysInfo() {
        delete u;
        delete disk_info_;
        delete mem_info_;
        delete s_;
        delete t_;
}

void ThreadOutSysInfo::stop() {
    exit_flag_ = 1;
    t_->join();
}
