//
// Created by bysou on 2021/10/17.
//

#include "Buffer.h"

//! 构造函数
//! \param capacity
Buffer::Buffer(int capacity) : capacity_(capacity), length_(0){
    print("构造器");
    buf_ = capacity_ == 0 ? nullptr: new unsigned char [capacity]{};
}

//! 重载拷贝运算符
//! \param buffer
//! \return
Buffer &Buffer::operator=(const Buffer &buffer) {
    if(&buffer!= this){
        print("拷贝运算符");
        this->capacity_ = buffer.capacity_;
        this->length_ = buffer.length_;
        delete[] this->buf_;
        this->buf_ = new unsigned char[buffer.capacity_];
        std::copy(buffer.buf_,buffer.buf_ + buffer.capacity_, this->buf_);
    }
    return *this;
}

//! 拷贝构造器
//! \param buffer
Buffer::Buffer(Buffer &buffer):Buffer(0){

    print("拷贝构造器");
#if 0
    this->capacity_ = buffer.capacity_;
    this->length_ = buffer.length_;
    this->buf_ = new unsigned char [capacity_];
    std::copy(buffer.buf_, buffer.buf_+buffer.capacity_, this->buf_);
#else
    *this = buffer;

#endif
}

Buffer::~Buffer() {
    print("析构");
    delete [] this->buf_;

}

std::ostream &operator<<(std::ostream &os, Buffer &buffer) {

    os << "Buffer(" << buffer.length_ << "/" << buffer.capacity_ << ")[";
    for (int i = 0; i < buffer.capacity_; ++i) {
        os << (int ) buffer.buf_[i] <<",";
    }
    os << "]";
    return os;

}

bool Buffer::write(unsigned char value) {
    if(length_ == capacity_){
        return false;
    }
    buf_[length_++] = value;
    return true;
}


Buffer::Buffer(Buffer &&buffer)  noexcept :capacity_(0), length_(0),
                                           buf_(nullptr) {
    print("移动构造器");
#if 0
    if(this != &buffer){
        this->capacity_ = buffer.capacity_;
        this->length_ = buffer.length_;
        this->buf_ = buffer.buf_;

        buffer.capacity_ = 0;
        buffer.length_ = 0;
        buffer.buf_ = nullptr;

    }
#else
//    *this = buffer;
    *this = std::move(buffer);
#endif
}

Buffer &Buffer::operator=(Buffer &&buffer) noexcept{
    print("移动拷贝");
    if(this != &buffer){
        this->capacity_ = buffer.capacity_;
        this->length_ = buffer.length_;
        delete [] this->buf_;
        this->buf_ = buffer.buf_;

        buffer.capacity_ = 0;
        buffer.length_ = 0;
        buffer.buf_ = nullptr;

    }
    return *this;
}
