//
// Created by bysou on 2021/9/21.
//

#include "gtest/gtest.h"
#include "TempClass.h"
#include "Buffer.h"

TEST(hellow, out){
    printf("hello wolrd\r\n");
}

TEST(bind, out){
    THREAD_TEST t;
    auto func = std::bind(&THREAD_TEST::func1, &t, std::placeholders::_1);
    auto func2 = std::bind(&THREAD_TEST::func2, &t, std::placeholders::_1);
    func(1);
    func2(2);
    t.print_m_a1();
}

TEST(buffer, buf){
    Buffer buf(10);
    Buffer buf1 = buf;
    buf1.write('z');
    std::cout << buf << std::endl;
    std::cout << buf1 << std::endl;
//
//    Buffer buf2(buf1);
//    buf2 = buf1;
//    std::cout << buf2 << std::endl;
//    std::cout << buf2 << std::endl;
//    Buffer* buf3 = &buf1;
    std::cout<<"====="<<std::endl;
    Buffer buf4(10);
    buf4 = Buffer(16);

    std::vector<Buffer> vec_buf;
    vec_buf.emplace_back(10);
    vec_buf.emplace_back(12);

    Buffer buf5{20};

    println(1,2,3,4,5);

}