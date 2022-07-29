//
// Created by bys on 2022/7/5.
//

#ifndef BYS_LIB_OPENPANO_TESTS_FEATURE_DIST_H_
#define BYS_LIB_OPENPANO_TESTS_FEATURE_DIST_H_

#include <limits>
#include "../lib/debugutils.h"

namespace pano {

//! 欧几里得距离  y=\sqrt{\sum_{i=1}^{n}{\left| x_{i}-y_{i} \right|^{2}}}
//! \param x
//! \param y
//! \param n
//! \param now_thres
//! \return
float euclidean_sqr(
    const float* x, const float* y,
    size_t n, float now_thres);

//! 汉明距离
//! \param x
//! \param y
//! \param n
//! \return
int hamming(const float* x, const float* y, int n);

// a L2 implementation compatible with FLANN to use
// work for float array of size 4k
struct L2SSE {
  typedef bool is_kdtree_distance;
  typedef float ElementType;
  typedef float ResultType;

  template <typename Iterator1, typename Iterator2>
  inline float operator()(
      Iterator1 a, Iterator2 b,
      size_t size, ResultType worst_dist = -1) const {
    if (worst_dist <= 0) worst_dist = std::numeric_limits<float>::max();
    return pano::euclidean_sqr(a, b, size, worst_dist);
  }

  template <typename U, typename V>
  inline ResultType accum_dist(const U& a, const V& b, int) const {
    return (a-b)*(a-b);
  }
};

}

#endif //BYS_LIB_OPENPANO_TESTS_FEATURE_DIST_H_
