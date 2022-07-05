//
// Created by bys on 2022/6/27.
//

#include "matrix.h"

#include <algorithm>
#include <memory>
#include <Eigen/Dense>

#include "geometry.h"
#include "timer.h"

using namespace std;

//TODO speedup at() to avoid channel mult

namespace {
//! 转换到 Eigen::Map
//! \param m    matrix: row * col * channel
//! \return     Eigen::Map<Eigen::Matrix>
inline Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>
to_eigenmap(const Matrix& m) {
  return Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(
      (double*)m.ptr(), m.rows(), m.cols());
}
}

ostream& operator << (std::ostream& os, const Matrix & m) {
  os << "[" << m.rows() << " " << m.cols() << "] :" << endl;
  REP(i, m.rows()) REP(j, m.cols())
  os << m.at(i, j) << (j == m.cols() - 1 ? "\n" : ", ");
  return os;
}

//! 转置矩阵    m x n => n x m
//! \return
Matrix Matrix::transpose() const {
  Matrix ret(m_cols, m_rows);
  REP(i, m_rows) REP(j, m_cols)
  ret.at(j, i) = at(i, j);
  return ret;
}

//! 矩阵乘积 C = AB A_r*B_c
//! \param r
//! \return
Matrix Matrix::prod(const Matrix & r) const {
  using namespace Eigen;
  Matrix ret(m_rows, r.cols());
  auto m1 = to_eigenmap(*this),
      m2 = to_eigenmap(r),
      res = to_eigenmap(ret);
  res = m1 * m2;
  return ret;
}

//! hadamard prod   C = A * B  A_ij*B_ij
//! \param r
//! \return
Matrix Matrix::elem_prod(const Matrix& r) const {
  m_assert(m_rows == r.rows() && m_cols == r.cols());
  Matrix ret(m_rows, m_cols);
  double* res = ret.ptr();
  const double *rl = ptr(), *rr = r.ptr();
  REP(i, pixels()) res[i] = rl[i] * rr[i];
  return ret;
}

//! 矩阵 - (Q = Q - R)
//! \param r     待减矩阵
//! \return
Matrix Matrix::operator - (const Matrix& r) const {
  m_assert(rows() == r.rows() && cols() == r.cols());
  Matrix ret(rows(), cols());
  double* res = ret.ptr();
  const double *rl = ptr(), *rr = r.ptr();
  REP(i, pixels()) res[i] = rl[i] - rr[i];
  return ret;
}

//! 矩阵 + (Q = Q + R)
//! \param r     待加矩阵
//! \return
Matrix Matrix::operator + (const Matrix& r) const {
  m_assert(rows() == r.rows() && cols() == r.cols());
  Matrix ret(rows(), cols());
  double* res = ret.ptr();
  const double *rl = ptr(), *rr = r.ptr();
  REP(i, pixels()) res[i] = rl[i] + rr[i];
  return ret;
}

//! 逆矩阵 AB = BA = I
//! \param ret
//! \return
bool Matrix::inverse(Matrix &ret) const {
  m_assert(m_rows == m_cols);
  using namespace Eigen;
  ret = Matrix(m_rows, m_rows);
  auto input = to_eigenmap(*this),
      res = to_eigenmap(ret);
  FullPivLU<Eigen::Matrix<double,Dynamic,Dynamic,RowMajor>> lu(input);
  if (! lu.isInvertible()) return false;
  res = lu.inverse().eval();
  return true;
}

//! 广义逆矩阵   AGA = A
//! \return
// pseudo inverse by SVD
Matrix Matrix::pseudo_inverse() const {
  using namespace Eigen;
  m_assert(m_rows >= m_cols);
  auto input = to_eigenmap(*this);
  JacobiSVD<MatrixXd> svd(input, ComputeThinU | ComputeThinV);
  auto sinv = svd.singularValues();
  REP(i, m_cols) {
    if (sinv(i) > EPS)
      sinv(i) = 1.0 / sinv(i);
    else
      sinv(i) = 0;
  }
  Matrix ret(m_cols, m_rows);
  auto res = to_eigenmap(ret);
  res = svd.matrixV() * sinv.asDiagonal() * svd.matrixU().transpose();
  m_assert(ret.at(0, 2) == res(0, 2));
  return ret;
}

//!
void Matrix::normrot() {
  m_assert(m_cols == 3);
  Vec p(at (0, 0), at(1, 0), at(2, 0));
  Vec q(at(0, 1), at(1, 1), at(2, 1));
  Vec r(at(0, 2), at(1, 2), at(2, 2));
  p.normalize();
  q.normalize();
  r.normalize();
  Vec vtmp = p.cross(q);
  double dist = (vtmp - r).mod();
  if (dist > 1e-6)
    r = vtmp;
  at(0, 0) = p.x, at(1, 0) = p.y, at(2, 0) = p.z;
  at(0, 1) = q.x, at(1, 1) = q.y, at(2, 1) = q.z;
  at(0, 2) = r.x, at(1, 2) = r.y, at(2, 2) = r.z;
}

//! 计算列平方和
//! \return
double Matrix::sqrsum() const {
  m_assert(m_cols == 1);
  double sum = 0;
  REP(i, m_rows) {
    cout << at(i, 0) << " ";
    sum += sqr(at(i, 0));
  }
  return sum;
}

//! 获取矩阵列
//! \param i
//! \return
Matrix Matrix::col(int i) const {
  m_assert(i < m_cols);
  Matrix ret(m_rows, 1);
  REP(j, m_rows)
  ret.at(j, 0) = at(j, i);
  return ret;
}

//! 单位矩阵
//! \param k     阶
//! \return
Matrix Matrix::I(int k) {
  Matrix ret(k, k);
  ret.zero();
  REP(i, k)
  ret.at(i, i) = 1;
  return ret;
}

//! 清空矩阵
void Matrix::zero() {
  double* p = ptr();
  int n = pixels();
  memset(p, 0, n * sizeof(double));
}
