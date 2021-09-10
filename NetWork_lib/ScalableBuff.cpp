//
// Created by ubuntu on 9/1/21.
//

#include "ScalableBuff.h"

ScalableBuff::ScalableBuff(unsigned int len) : buf_size_(len){
    create_vecbuf();
}

int ScalableBuff::create_vecbuf() {
    this->vecbuff_ = (VecBuff_t*)malloc(sizeof(VecBuff_t));
    if(vecbuff_ == nullptr){
        return VecBuff_CREATE_ERROR;
    }

    vecbuff_->data = (char *) malloc(buf_size_);
    if(vecbuff_->data == nullptr){
        return VecBuff_DATA_CREATE_ERROR;
    }

    vecbuff_->used = 0;
    vecbuff_->capacity = buf_size_;
}

int ScalableBuff::push_vecbuf(const char *data, unsigned int len) {
    int ret_val = 0;
    int fail_cnt = 0;
    if(vecbuff_ == nullptr || data == nullptr){
        return VecBuff_EXIST_ERROR;
    }

    while (len > (vecbuff_->capacity - vecbuff_->used)){
        ret_val = esxtend_vecbuf();
        if(ret_val < 0){
            fail_cnt ++;
        } else{
            fail_cnt = 0;
        }

        if (fail_cnt > 10){
            return VecBuff_DATA_EXTERN_ERROR;
        }

    }
    memcpy(vecbuff_->data + vecbuff_->used, data, len);
    vecbuff_->used += len;
    return 0;
}

int ScalableBuff::read_vecbuf(unsigned int offset, char *data, unsigned int len) {
    unsigned int Valid_len, read_len;
    if(nullptr == vecbuff_ || nullptr == data){
        return VecBuff_READ_PARAM_ERROR;
    }
    if (offset > vecbuff_->used){
        return VecBuff_READ_LEN_ERROR;
    }

    Valid_len = vecbuff_->used - offset;
    read_len = (len > Valid_len) ? Valid_len : len;
    memcpy(data, vecbuff_->data + offset, read_len);
    return read_len;
}

int ScalableBuff::esxtend_vecbuf() {
    void * tmp = nullptr;
    tmp = realloc(vecbuff_->data,vecbuff_->capacity*2);
    if(tmp == nullptr){
        return VecBuff_DATA_EXTERN_ERROR;
    }
    vecbuff_->data = (char *)tmp;
    vecbuff_->capacity*=2;
    return vecbuff_->capacity;
}

ScalableBuff::~ScalableBuff() {
    if(vecbuff_!= nullptr){
        if(vecbuff_->data != nullptr){
            free(vecbuff_->data);
        }
        free(vecbuff_);
    }
}

unsigned int ScalableBuff::get_used() {
    return vecbuff_->used;
}

unsigned int ScalableBuff::get_capacity() {
    return vecbuff_->capacity;
}

char *ScalableBuff::get_data() {
    return vecbuff_->data;
}


