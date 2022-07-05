//
// Created by bys on 2022/6/27.
//

#ifndef BYS_LIB_OPENPANO_TESTS_LIB_GEOMETRY_H_
#define BYS_LIB_OPENPANO_TESTS_LIB_GEOMETRY_H_

#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include "utils.h"
#include "debugutils.h"

//! 几何类
class Geometry {
 public:
  int w, h;

  Geometry(int m_w = 0, int m_h = 0) :
      w(m_w), h(m_h) {}

  //! 面积
  //! \return
  int area() const { return w * h; }

  //! 长宽比
  //! \return
  real_t ratio() const { return (real_t) std::max(w, h) / std::min(w, h); }

  //! x,y 在几何内部
  //! \param x
  //! \param y
  //! \return
  bool contain(int x, int y) { return (x >= 0 && x < w && y >= 0 && y < h); }
};

template<typename T>
class Vector {
 public:
  T x = 0, y = 0, z = 0;

  constexpr explicit Vector(T m_x = 0, T m_y = 0, T m_z = 0) :
      x(m_x), y(m_y), z(m_z) {}

  constexpr Vector(const Vector<T> &rhs) = default;

  Vector(const Vector &p0, const Vector &p1) :
      x(p1.x - p0.x), y(p1.y - p0.y), z(p1.z - p0.z) {}

  explicit Vector(const T *p) :
      x(p[0]), y(p[1]), z(p[2]) {}

  //! vector index
  //! \param c
  //! \return
  T index(int c) const { return c == 0 ? x : c == 1 ? y : z; }

  T &index(int c) { return c == 0 ? x : c == 1 ? y : z; }

  //! 返回绝对值最小值
  //! \return
  T min_comp_abs() const {
    T a = fabs(x), b = fabs(y), c = fabs(z);
    ::update_min(a, b), ::update_min(a, c);
    return a;
  }

  //! 平方
  //! \return
  T sqr() const { return x * x + y * y + z * z; }

  //! 取模
  //! \return
  T mod() const { return sqrt(sqr()); }

  //! 点积  a·b
  //! \param v
  //! \return
  T dot(const Vector &v) const { return x * v.x + y * v.y + z * v.z; }

  //! 叉积  axb
  //! \param v
  //! \return
  Vector cross(const Vector &v) const { return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

  Vector &operator=(const Vector &v) {
    x = v.x, y = v.y, z = v.z;
    return *this;
  }

  //! 标准化
  void normalize() {
    T m = 1 / mod();
    *this *= m;        // work?
    m_assert(std::isnormal(m));
  }

  //! 返回标准化vector
  //! \return
  Vector get_normalized() const {
    Vector ret(*this);
    ret.normalize();
    return ret;
  }

  //! 零向量判断
  //! \param threshold
  //! \return
  bool is_zero(T threshold = EPS) const { return fabs(x) < threshold && fabs(y) < threshold && fabs(z) < threshold; }

  //! 正向量判断
  //! \param threshold
  //! \return
  bool is_positive(T threshold = EPS) const { return x > threshold && y > threshold && z > threshold; }

  //! 更新最小值
  //! \param v
  void update_min(const Vector &v) {
    ::update_min(x, v.x);
    ::update_min(y, v.y);
    ::update_min(z, v.z);
  }

  //! 更新最大值
  //! \param v
  void update_max(const Vector &v) {
    ::update_max(x, v.x);
    ::update_max(y, v.y);
    ::update_max(z, v.z);
  }

  Vector operator+(const Vector &v) const { return Vector(x + v.x, y + v.y, z + v.z); }

  Vector &operator+=(const Vector &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }

  Vector operator-() const { return Vector(-x, -y, -z); }

  Vector &operator-=(const Vector &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  Vector operator*(T p) const { return Vector(x * p, y * p, z * p); }

  Vector &operator*=(T p) {
    x *= p;
    y *= p;
    z *= p;
    return *this;
  }

  Vector operator/(T p) const { return *this * (1.0 / p); }

  Vector &operator/=(T p) {
    x /= p;
    y /= p;
    z /= p;
    return *this;
  }

  bool operator==(const Vector &v) const { return fabs(x - v.x) < EPS && fabs(y - v.y) < EPS && fabs(z - v.z) < EPS; }

  bool operator!=(const Vector &v) const {
    return fabs(x - v.x) >= EPS || fabs(y - v.y) >= EPS || fabs(z - v.z) >= EPS;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector &vec) {
    return os << vec.x << " " << vec.y << " " << vec.z;
  }

  //  返回max vector
  static Vector max() { return Vector(std::numeric_limits<T>::max(), std::numeric_limits<T>::max()); }

  //  返回inf vector
  static Vector infinity() { return Vector(std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity()); }

  T get_max() const { return std::max(x, std::max(y, z)); }

  T get_min() const { return std::min(x, std::min(y, z)); }

  T get_abs_max() { return std::max(fabs(x), std::max(fabs(y), fabs(z))); }

  //! vector 写入数组
  //! \param p    out arr
  void write_to(T *p) const { p[0] = x, p[1] = y, p[2] = z; }

  static Vector get_zero() { return Vector(0, 0, 0); }

  // i'm norm
  //! 反射向量    𝑟 = 2 ( 𝑑 ⋅ 𝑛 ) 𝑛 - 𝑑
  //! \param v
  //! \return
  Vector reflection(const Vector &v) const {
    m_assert(fabs(v.sqr() - 1) < EPS && (sqr() - 1 < EPS));
    return *this * 2 * dot(v) - v;
  }
};

template<typename T>
class Vector2D {
 public:
  T x = 0, y = 0;

  Vector2D<T>() {};

  explicit Vector2D<T>(T m_x, T m_y) :
      x(m_x), y(m_y) {}

  Vector2D<T>(const Vector2D<T> &p0, const Vector2D<T> &p1) :
      x(p1.x - p0.x), y(p1.y - p0.y) {}

  //! 点积  a·b
  //! \param v
  //! \return
  T dot(const Vector2D<T> &v) const { return x * v.x + y * v.y; }

  //! 差积  axb
  //! \param v
  //! \return
  T cross(const Vector2D<T> &v) const { return x * v.y - y * v.x; }

  //! a = a + b
  //! \param v
  //! \return
  Vector2D<T> operator+(const Vector2D<T> &v) const { return Vector2D<T>(x + v.x, y + v.y); }

  //! a += b
  //! \param v
  //! \return
  Vector2D<T> &operator+=(const Vector2D<T> &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  //! a = a - b
  //! \param v
  //! \return
  Vector2D<T> operator-(const Vector2D<T> &v) const { return Vector2D<T>(x - v.x, y - v.y); }

  //! a = -a
  //! \return
  Vector2D<T> operator-() const { return Vector2D<T>(-x, -y); }

  //! a -= b
  //! \param v
  //! \return
  Vector2D<T> &operator-=(const Vector2D<T> &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  //! a = a * T
  //! \param f
  //! \return
  Vector2D<T> operator*(T f) const { return Vector2D<T>(x * f, y * f); }

  //! a *= T
  //! \param p
  //! \return
  Vector2D<T> &operator*=(T p) {
    x *= p;
    y *= p;
    return *this;
  }

  //! a = a / T
  //! \param f
  //! \return
  Vector2D<T> operator/(T f) const { return *this * (1.0 / f); }

  //! a = a * b
  //! \param v
  //! \return
  Vector2D<T> operator*(const Vector2D<T> &v) const { return Vector2D<T>(x * v.x, y * v.y); }

  //! a = a / b
  //! \param v
  //! \return
  Vector2D<T> operator/(const Vector2D<T> &v) const { return Vector2D<T>(x / v.x, y / v.y); }

  //! a == b ?
  //! \param v
  //! \return
  bool operator==(const Vector2D<T> &v) const { return fabs(x - v.x) < EPS && fabs(y - v.y) < EPS; }

  //! 取 第二部 负数
  //! \return
  // take negative of the second component
  Vector2D<T> operator!() const { return Vector2D<T>(x, -y); }

  //! 交换 第一部和第二部
  //! \return
  // swap the two component
  Vector2D<T> operator~() const { return Vector2D<T>(y, x); }

  bool is_zero() const { return fabs(x) < EPS && fabs(y) < EPS; }

  T sqr() const { return x * x + y * y; }

  //! 取模
  //! \return
  T mod() const { return hypot(x, y); }

  //! 归一化
  //! \return
  Vector2D<T> get_normalized() const {
    T m = mod();
    m_assert(m > EPS);
    m = 1.0 / m;
    return Vector2D<T>(x * m, y * m);
  }

  void normalize() {
    T m = (T) 1.0 / mod();
    x *= m, y *= m;        // work?
    m_assert(std::isnormal(m));
  }

  template<typename TT>
  friend std::ostream &operator<<(std::ostream &os, const Vector2D<TT> &v);

  void update_min(const Vector2D<T> &v) {
    ::update_min(x, v.x);
    ::update_min(y, v.y);
  }

  void update_max(const Vector2D<T> &v) {
    ::update_max(x, v.x);
    ::update_max(y, v.y);
  }

  bool isNaN() const { return std::isnan(x); }

  static Vector2D<T> NaN() { return Vector2D<T>(NAN, NAN); }

  static Vector2D<T> max() {
    return Vector2D<T>(
        std::numeric_limits<T>::max(),
        std::numeric_limits<T>::max());
  }
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const Vector2D<T> &v) {
  os << v.x << ' ' << v.y;
  return os;
}

typedef Vector<double> Vec;
typedef Vector2D<int> Coor;
typedef Vector2D<double> Vec2D;

#endif //BYS_LIB_OPENPANO_TESTS_LIB_GEOMETRY_H_
