//
// Created by ubuntu on 5/24/21.
//

#include "FileOpt.h"

std::string FileOpt::get_current_url() {
#if 1
    char url[kStdUrlSize] = {};
    if (!getcwd(url, kStdUrlSize)) {
//
        return nullptr;
    }
    return std::__cxx11::string(url);

#if 0
    else {
//        std::string str_url;
//        str_url.append(url);
//        return str_url;

    }
#endif
#endif
#if 0
    char abs_path[1024] = {};
    int cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
    if (cnt < 0 || cnt >= 1024) {
        return NULL;
    }

    //最后一个'/' 后面是可执行程序名，去掉devel/lib/m100/exe，只保留前面部分路径
    for (int i = cnt; i >= 0; --i) {
        if (abs_path[i] == '/') {
            abs_path[i + 1] = '\0';
            break;
        }
    }

    std::string url(abs_path);
    _print("log system abs url : %s\n", url.data());
    return url;
#endif
    return std::__cxx11::string(url);
}

int FileOpt::judge_dir_exist_and_create(const std::string &file_url) {
    if(file_url.size() == 0){
        return kInvalid;
    }

    char flag[4] = "/";
    int pos = 0;
//    int i = 0;
    while ((pos = file_url.find(flag, pos)) != std::string::npos){
        if(pos != 0){
            char sub_url[pos+1];
            memset(sub_url,0,sizeof (sub_url));
            memcpy(sub_url, file_url.data(), pos);
            if(access(sub_url,F_OK) != 0){
                mkdir(sub_url, 0775);
            }
            _print("file :%s\n", sub_url);
        }
        pos++;
    }
    return 0;
}

int FileOpt::judge_dir_exist_and_create(const char *file_url) {
#if 0
    if(file_url == nullptr){
        return kInvalid;
    }

    char present_url[kStdUrlSize]={};
    char present_url2[kStdUrlSize]={};

    const char *pos = nullptr;

    while ((pos = strstr(file_url,"/"))!= nullptr){
        memcpy(present_url, file_url,  (pos - &file_url[0] + 1 ) );
        strcat(present_url2, present_url);
        file_url += ( pos - file_url + 1);

        memset(present_url, 0, kStdUrlSize);
        if( access(present_url2, F_OK) != 0 ){
            mkdir(present_url2,0775);
        }
        _print("%s\n", present_url2);

    }
#endif
#if 01
    if(file_url == nullptr){
        return kInvalid;
    }
    char flag[4] = "/";

    char present_url[kStdUrlSize]={};
    char present_url2[kStdUrlSize]={};
    memcpy(present_url2, file_url, strlen(file_url)+1);
    int len = strlen(file_url);
    int index = 0;
    const char *pos = nullptr;

    while ((pos = strstr(file_url,"/"))!= nullptr){
        index = (len - strlen(pos));
        if(index != 0){
            memcpy(present_url, present_url2,  index );
            _print("present_url: %s\n", present_url);
            if(access(present_url,F_OK) != 0 ){
                mkdir(present_url, 0775);
            }
        }

        file_url += ( pos - file_url + 1);

    }

#endif
}
