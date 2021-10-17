//
// Created by bysou on 2021/10/17.
//
//

#ifndef BYS_LIB_BUFFER_H
#define BYS_LIB_BUFFER_H

#include <iostream>
#include <stdio.h>

template<typename T>
void print(T t){
    std::cout<< t << std::endl;
}

template<typename T,typename ...U>
void print(T t,U...u){
    std::cout << t << " ";
    print(u...);
}

template<typename T>
void println(std::initializer_list<T> args ) {
    for(auto arg:args){
        std::cout << arg << ", ";
    }


}

class Buffer {
public:
    explicit Buffer(int capacity);
    Buffer(Buffer& buffer);
    Buffer(Buffer&& buffer) noexcept  ;
    Buffer &operator=(Buffer const &buffer);
    Buffer &operator=(Buffer &&buffer) noexcept  ;


    bool write(unsigned char value);

    friend  std::ostream &operator<<(std::ostream &os, Buffer &buffer);
    ~Buffer();

private:
    unsigned char *buf_;
    int capacity_;
    int length_;

};


#endif //BYS_LIB_BUFFER_H
