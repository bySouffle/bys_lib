//
// Created by bys on 2022/1/12.
//

#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <cstdio>

//! 1. 不在头文件中添加 using namespace std; 污染作用域
TEST(No_1, namespace_std) {
    using namespace std;
    string s{"hello world!"};
    cout << s << endl;
}

//! 2. 不在循环中使用std::cout 它将刷新缓冲区, 使用 \n
TEST(No_2, std_cout) {
    int cnt = 10;
    for (int i = 0; i < cnt; ++i) {
        std::cout << i << "\n";
    }
}

//! 3. 在for循环中不关心索引时
TEST(No_3, for_iterator) {
    std::vector<int> vec{1, 2, 3, 4, 5};
//  NG
//    for (int i = 0; i < vec.size(); ++i) {
//        std::cout << vec[i] << "\n";
//    }
    for (auto &it: vec) {
        std::cout << it << "\n";
    }

}

//! 4. 查找某个符合条件的元素使用 std::find_if
TEST(No_4, search_item) {
    const std::vector<int> vec{-1, -2, 3, 4, -5};
    //  (1) by yourself
    std::size_t first_index;
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] > 0) {
            first_index = i;
            break;
        }
    }

    //  (2) by std
    const auto is_positive = [](const auto &x) { return x > 0; };
    auto first_index_2 = std::find_if(vec.cbegin(), vec.cend(), is_positive);
    printf("by std: index = %d\n", *first_index_2);
}

//  5. 标准数组作为参数传递通常要传递数组指针与数组长度，使用std::array
#include <array>

template<std::size_t size>
int cnt_arr_sum(std::array<int, size> &arr) {
    int cnt = 0;
    for (auto &it: arr) {
        cnt += it;
    }
    return cnt;
}

TEST(No_5, std_array) {
    //  (1) use int arr[]   这种方式存在的问题在于在初始化的时候如果未完全初始化， 不知道其中有多少个有效元素
    const int n = 8;
    int arr[n] = {1, 2, 3};

    auto f = [](const int *arr, int n) -> int {
        int cnt = 0;
        for (int i = 0; i < n; ++n) {
            cnt += arr[i];
        }
        return cnt;
    };

    std::cout << f(arr, n) << std::endl;

    //  (2) use std::array
    const int n2 = 8;
    std::array<int, n> arr2{1, 2, 3};
    std::cout << cnt_arr_sum(arr2) << std::endl;

}

//! 6. 使用   reinterpret_cast<type>(exp)
//!     reinterpret_cast 表达式不会编译成任何 CPU 指令（除非在整数和指针间转换，或在指针表示依赖其类型的不明架构上）。
//!     它纯粹是一个编译时指令，指示编译器将 表达式 视为如同具有 新类型 类型一样处理。
#include <cstddef>
#include <bit>
#include <bitset>
template<typename T>
void print_bytes(const T &input) {
    //   bug in arm
    /*
    using bytearray = std::array<std::byte, sizeof (T)>;
    const auto &bytes = std::bit_cast<bytearray, T>(input);
    */
}

TEST(No_6, reinterpret_cast_) {
    long long x = 0;
    auto xp = reinterpret_cast<char *>(x);
    auto x2 = reinterpret_cast<long long >(xp);
    std::cout << x2 << std::endl;

    //  c style
    float y = .123f;
    long i = *(long *) &y;
    y = *(float *) &i;
    std::cout << y << std::endl;

    //  C++17
//    typedef std::bitset<std::numeric_limits<unsigned char>::digits> ByteBitset;
    using ByteBitset = std::bitset<std::numeric_limits<unsigned char>::digits>;
    std::byte b1{0b1111'0000};
    std::cout << ByteBitset{std::to_integer<unsigned char>(b1)} << std::endl;

    auto *bytes = reinterpret_cast<const std::byte *>(&y);

    std::cout << ByteBitset{std::to_integer<unsigned char>(*bytes)} << std::endl;

    std::cout << std::to_integer<int>(*bytes) << std::endl;

    //  C++20
//    print_bytes(xx);

}

//! 7. 在 map 中 查找用 at 代替 []
//!     map 中重复元素用insert不会生效 用[]相当于重载会覆盖
#include <unordered_map>

const std::string &cnt_times(const std::unordered_map<std::string, int> &word_counts,
                             const std::string &word1, const std::string &word2) {
    //  err 若 map[word] 不存在则直接插入到map中
//    return word_counts[word1] > word_counts[word2] ? word1 : word_counts;

    //
//    auto &counts = const_cast<std::unordered_map<std::string, int >&>(word_counts);
//    return counts[word1] > counts[word2] ? word1 : word2;

    return word_counts.at(word1) > word_counts.at(word2) ? word1 : word2;

}

TEST(No_7, map_const) {
    std::unordered_map<std::string, int> word_cnt{{"zz", 2},
                                                  {"aa", 5},
                                                  {"zz", 6}};
    std::string zz("zz");
    std::string aa("aa");

    word_cnt["a"];  //  若a不存在则直接插入
//    word_cnt.at("a?");    //  使用 at 若 a 不存在则抛出错误

    std::cout << cnt_times(word_cnt, zz, aa) << std::endl;
}

//!  8. 在不修改变量的函数参数上添加const
void print_vec_per_line(const std::vector<int> &arr) {
    for (const auto &x: arr) {
        std::cout << x << "\n";
    }
}

TEST(No_8, const_param) {
    std::vector<int> arr{1, 2, 3, 4, 5};
    print_vec_per_line(arr);
}

//!  9. 局部变量的生命周期
const char *string_lifetimes() {
    return "zz";
}

TEST(No_9, string_lifetimes) {
    std::cout << string_lifetimes() << "\n";
}

//!  10. 不使用结构化绑定
TEST(No_10, it_bind) {
    std::unordered_map<std::string, std::string> colors{
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"},

    };

    for (const auto &pair: colors) {
        std::cout << "color " << pair.first << " hex " << pair.second << " \n";
    }

    for (const auto&[color, hex]: colors) {
        std::cout << "color " << color << " hex " << hex << " \n";
    }

    struct S {
        int a;
        std::string s;
    };

    auto f = []() -> S {
        S s{1, "zz"};
        return s;
    };

    const auto[id, str] = f();
    std::cout << id << " " << str << "\n";
}

//!  11. 返回多个值,使用结构体
TEST(No_11, get_value_return_struct) {
    struct Value_11 {
        int a, b;

        void Print() const {
            std::cout << "a " << a << " b " << b << "\n";
        }
    };
//    Value_11 get_val_return_struct(const int a){
//
//    };

    auto f = [](const int a) -> Value_11 {
        return {a, a + 1};
    };

    auto val11 = f(1);
    val11.Print();
}

//! 12. constexpr在函数中已知要先计算的表达式
constexpr int calc_1_to_n_sum(const int n) {
    return n * (n + 1) / 2;
}

TEST(No_12, constexpr_use) {
    const int n = 1000;
    auto sum_n = calc_1_to_n_sum(n);
    std::cout << "sum " << sum_n << '\n';
}

//! 13. 派生类的析构函数标记 override 基类的析构函数标记为 virtual
//!     父类函数释放派生类时，如果没有虚构析构函数和重载析构函数
//!     则只会调用父类析构函数
class Base_class {
public:
    Base_class() {
        std::cout << "Base class\n";
    }

    virtual ~Base_class() {
//    ~Base_class(){
        std::cout << "~Base class\n";
    }

    virtual void Print() {
        std::cout << "print Base class\n";
    }
};

class Derived_class : public Base_class {
public:
    Derived_class() {
        std::cout << "Derived class\n";
    }

    ~Derived_class() override {
//    ~Derived_class(){
        std::cout << "~Derived class\n";
    }

    void Print() override {
        std::cout << "print Derived class\n";
    }
};

TEST(No_13, derived_class) {
    auto *dev = new Derived_class();

    dev->Print();
    Base_class *b_dev = dev;
    std::cout << "===========\n";
    b_dev->Print();

//    delete dev;
    std::cout << "===========\n";
    delete b_dev;
}
