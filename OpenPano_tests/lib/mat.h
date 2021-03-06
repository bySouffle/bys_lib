//
// Created by bys on 2022/6/26.
//

#ifndef BYS_LIB_OPENPANO_TESTS_LIB_MAT_H_
#define BYS_LIB_OPENPANO_TESTS_LIB_MAT_H_

#include <memory>
#include <cstring>
#include "debugutils.h"

//! n维矩阵
//! \tparam T 矩阵内元素type
template <typename T>
class Mat {
 public:
  Mat(){}
  Mat(int rows, int cols, int channels):
      m_rows(rows), m_cols(cols), m_channels(channels),
      m_data{new T[rows * cols * channels], std::default_delete<T[]>() }
  { }

  virtual ~Mat(){}

  T &at(int r, int c, int ch = 0) {
    m_assert(r < m_rows);
    m_assert(c < m_cols);
    m_assert(ch < m_channels);
    return ptr(r)[c * m_channels + ch];
  }

  const T &at(int r, int c, int ch = 0) const {
    m_assert(r < m_rows);
    m_assert(c < m_cols);
    m_assert(ch < m_channels);
    return ptr(r)[c * m_channels + ch];
  }

  Mat<T> clone() const {
    Mat<T> res(m_rows, m_cols, m_channels);
    memcpy(res.ptr(0), this->ptr(0), sizeof(T) * m_rows * m_cols * m_channels);
    return res;
  }

  const T *ptr(int r = 0) const
  { return m_data.get() + r * m_cols * m_channels; }

  T *ptr(int r = 0)
  { return m_data.get() + r * m_cols * m_channels; }

  const T *ptr(int r, int c) const
  { return m_data.get() + (r * m_cols + c) * m_channels; }

  T *ptr(int r, int c)
  { return m_data.get() + (r * m_cols + c) * m_channels; }

  int height() const { return m_rows; }
  int width() const { return m_cols; }
  int rows() const { return m_rows; }
  int cols() const { return m_cols; }
  int channels() const { return m_channels; }
  int pixels() const { return m_rows * m_cols; }

 protected:
  int m_rows, m_cols; //  行列
  int m_channels; //  通道
  std::shared_ptr<T> m_data;  //  数据指针
};

using Mat32f = Mat<float>;
using Matuc = Mat<unsigned char>;


#endif //BYS_LIB_OPENPANO_TESTS_LIB_MAT_H_
