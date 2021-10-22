//
// Created by bysou on 2021/9/21.
//

#include "RedisTool.h"
#include "JsonDealClass.h"
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

    std::tuple<int,int,int,int,int> t = {1,2,3,4,5};
    println({1, 2, 3, 4});
}


TEST(redis, test){
    RedisTool r;
    r.init_redis();
    std::string z("ex1");
    std::vector<std::string> a = r.get_all_items(z);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }

    std::string key("ex1");
    std::string value("move6");

    //  zadd
    r.zADD_item(key,kMove, value);
    //  get all
    a = r.get_all_items(z);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }
    //  get type all
    a = r.get_type_items(key,kMove);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }
    std::vector<std::string> items;
    items.emplace_back("move7");
    items.emplace_back("move8");

    //  add vec
    print("%d\n", r.zADD_items(key, kMove, items, 2));
    a = r.get_type_items(key,kMove);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }

    //get size
    print("%d\n", r.get_size(key, kMove));
    print("del %d\n", r.del_type_items(key, kMove));
    print("%d\n", r.get_size(key, kMove));
    print("del %d\n", r.del_key(key));

    a = r.get_all_items(key);
    for(auto aa: a){
        std::cout<<aa<<std::endl;
    }
}

TEST(json_parse, test){
    std::string json(R"(aaa:[%, %, %] )");
    std::vector<double> a{1.1,2.2,3.21};
    double z = a.at(0);
    replace_json_symbol(json, a.at(0), a.at(1), a.at(2), 6);
    std::cout << json <<std::endl;

}