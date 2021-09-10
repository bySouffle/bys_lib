//
// Created by ubuntu on 9/1/21.
//

#ifndef BYS_LIB_SCALABLEBUFF_H
#define BYS_LIB_SCALABLEBUFF_H

#include <string.h>
#include <stdlib.h>

typedef struct VecBuff{
    unsigned int capacity;
    unsigned int used;

    char *data;
}VecBuff_t;

enum VecBuff_ERROR{
    VecBuff_BASE = -300,
    VecBuff_CREATE_ERROR        = VecBuff_BASE - 1,
    VecBuff_DATA_CREATE_ERROR   = VecBuff_BASE - 2,
    VecBuff_DATA_EXTERN_ERROR   = VecBuff_BASE - 2,
    VecBuff_EXIST_ERROR         = VecBuff_BASE - 2,
    VecBuff_READ_PARAM_ERROR    = VecBuff_BASE - 2,
    VecBuff_READ_LEN_ERROR      = VecBuff_BASE - 2,

};

enum VecBuffLimitSize{
    KBUFF_SIZE = 1024,

};

class ScalableBuff {
public:

    ScalableBuff(unsigned int len);
    ~ScalableBuff();
    int create_vecbuf();
//    void free_vecbuf();
    int push_vecbuf(const char*data, unsigned int len);
    int read_vecbuf(unsigned int offset, char *data, unsigned int len);
    int esxtend_vecbuf();

    unsigned int get_used();
    unsigned int get_capacity();
    char* get_data();
private:
    VecBuff_t *vecbuff_;
    unsigned int buf_size_;
};


#endif //BYS_LIB_SCALABLEBUFF_H
