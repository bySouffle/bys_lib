//
// Created by bys on 2022/6/26.
//

#ifndef BYS_LIB_OPENPANO_TESTS_LIB_UTILS_H_
#define BYS_LIB_OPENPANO_TESTS_LIB_UTILS_H_

#include <cstdarg>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <memory>
#include <sys/stat.h>
#include <type_traits>

#ifdef _WIN32
#define __attribute__(x)
#endif
//  精度控制 Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef double real_t;
const real_t EPS = 1e-6;
const real_t GEO_EPS_SQR = 1e-14;   //  几何精度平方
const real_t GEO_EPS = 1e-7;  //  几何精度
inline float sqr(float x) { return x * x; }
//  精度控制 End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//  for循环宏 REP Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define between(a, b, c) ((a >= b) && (a <= c - 1))
#define REP(x, y) for (typename std::remove_cv<typename std::remove_reference<decltype(y)>::type>::type x = 0; x < (y); x ++)
#define REPL(x, y, z) for (typename std::remove_cv<typename std::remove_reference<decltype(y)>::type>::type x = y; x < (z); x ++)
#define REPD(x, y, z) for (typename std::remove_cv<typename std::remove_reference<decltype(y)>::type>::type x = y; x >= (z); x --)
//  for循环宏 REP End   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//! 终端彩色输出
//! \param k (0-4)
//! \return green yellow blue magenta cyan
std::string TERM_COLOR(int k);

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

//! 彩色输出 fmt
//! \param col  TERM_COLOR(i).data()
//! \param fmt  format
//! \param ...  args
//! \sa         std::string TERM_COLOR(int k);
void c_printf(const char *col, const char *fmt, ...);

//! 输出格式化字符串
//! \param col  TERM_COLOR(i).data()
//! \param fp   文件描述符
//! \param fmt  format
//! \param ...  args
//! \sa         std::string TERM_COLOR(int k);
void c_fprintf(const char *col, FILE *fp, const char *fmt, ...);

//! 返回格式化字符串
//! \param fmt  format
//! \param ...  args
//! \return
__attribute__ (( format( printf, 1, 2 )))
std::string ssprintf(const char *fmt, ...);

//! 更新最小值
//! \tparam T   typename
//! \param dest 目标值
//! \param val  比较值
//! \return     true->update
template<typename T>
inline bool update_min(T &dest, const T &val) {
  if (val < dest) {
    dest = val;
    return true;
  }
  return false;
}
//! 更新最大值
//! \tparam T   typename
//! \param dest 目标值
//! \param val  比较值
//! \return     true->update
template<typename T>
inline bool update_max(T &dest, const T &val) {
  if (dest < val) {
    dest = val;
    return true;
  }
  return false;
}
//! 释放2级指针数组
//! \tparam T   typename
//! \param ptr  pointer
//! \param w    指针数组长度
template<typename T>
inline void free_2d(T **ptr, int w) {
  if (ptr != nullptr)
    for (int i = 0; i < w; i++)
      delete[] ptr[i];
  delete[] ptr;
}

//! 创建buff
//! \tparam T   buff类型
//! \param len  长度
//! \param init_zero    初始化缓存为0
//! \return     buff pointer
template<typename T>
std::shared_ptr<T> create_auto_buf(size_t len, bool init_zero = false) {
  std::shared_ptr<T> ret(new T[len], std::default_delete<T[]>());
  if (init_zero)
    memset(ret.get(), 0, sizeof(T) * len);
  return ret;
}

//! 判断文件是否存在
//! \param name 文件名
//! \return     1->存在
inline bool exists_file(const char *name) {
  struct stat buffer;
  return stat(name, &buffer) == 0;
}

//! 判断后缀名
//! \param str      文件名
//! \param suffix   后缀名
//! \return         1->ok
inline bool endswith(const char *str, const char *suffix) {
  if (!str || !suffix) return false;
  auto l1 = strlen(str), l2 = strlen(suffix);
  if (l2 > l1) return false;
  return strncmp(str + l1 - l2, suffix, l2) == 0;
}

#endif //BYS_LIB_OPENPANO_TESTS_LIB_UTILS_H_
