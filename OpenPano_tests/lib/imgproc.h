//
// Created by bys on 2022/6/30.
//

#ifndef BYS_LIB_OPENPANO_TESTS_LIB_IMGPROC_H_
#define BYS_LIB_OPENPANO_TESTS_LIB_IMGPROC_H_

#include <list>
#include "mat.h"
#include "color.h"

class Matrix;

namespace pano {
Mat32f read_img(const char* fname);
Matuc read_img_uc(const char* fname);
void write_rgb(const char* fname, const Mat32f& mat);
inline void write_rgb(const std::string s, const Mat32f& mat) { write_rgb(s.c_str(), mat); }

Mat32f hconcat(const std::list<Mat32f>& mats);
Mat32f vconcat(const std::list<Mat32f>& mats);

// interpolate color. return Color::NO if any of the neighbor is Color::NO
Color interpolate(const Mat32f& mat, float r, float c);

// interpolate color. return Color::NO only if r,c out of border
// return value still in [0,1]
Color interpolate(const Matuc& mat, float r, float c);

//! 减少信息
//! \param mat
//! \return
Mat32f crop(const Mat32f& mat);

Mat32f rgb2grey(const Mat32f& mat);

// get transform from p2 to p1
Matrix getPerspectiveTransform(const std::vector<Vec2D>& p1, const std::vector<Vec2D>& p2);
Matrix getAffineTransform(const std::vector<Vec2D>& p1, const std::vector<Vec2D>& p2);

void fill(Mat32f& mat, const Color& c);
void fill(Mat32f& mat, float c);

template <typename T>
void resize(const Mat<T> &src, Mat<T> &dst);

Matuc cvt_f2uc(const Mat32f& mat);
}


#endif //BYS_LIB_OPENPANO_TESTS_LIB_IMGPROC_H_
