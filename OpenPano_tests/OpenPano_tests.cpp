//
// Created by bys on 2022/6/23.
//

#include "gtest/gtest.h"
#include <typeinfo>
#include <stdarg.h>

#include "lib/utils.h"
using namespace std;

TEST(TEP_X, test) {
  std::cout << between(3, 2, 5) << "\n";
  double a = 5;
  int aa = 100;
  int i;
  REP(i, 5) aa++;
  std::cout << i << " " << a << "a " << aa << "\n";

  REPL(i, 2, 10) aa++;
  std::cout << i << " " << a << "a " << aa << "\n";

  REPD(i, 10, 2) aa--;
  std::cout << i << " " << a << "a " << aa << "\n";


  //  typename 修饰推导式为类型
  double db_0 = 0;
  typename std::remove_reference<decltype(db_0)>::type zz = 1;
  int status;
  const std::type_info &ti = typeid(zz);
  char *real_name = abi::__cxa_demangle(ti.name(), 0, 0, &status);
  std::cout << ti.name() << "\t=> " << real_name << "\t: " << status << '\n';
  free(real_name);

}

TEST(attr, test) {
  cout << ssprintf("%d %d %d\n", 1, 2, 3) << endl;
  cout << TERM_COLOR(0) << "zzzzz\n"; //  绿 黄 蓝 紫
  c_printf(TERM_COLOR(0).data(), "zzz%d\n", 123);
}

TEST(point_2d, del) {
  char **p = new char *[20]{};
  for (int i = 0; i < 20; ++i) {
    p[i] = new char[4];
    memcpy(p[i], ssprintf("%d%d%d", i, i, i).data(), 4);
    printf("%s [sizeof p[i] = %ld] [sizeof *p[i] = %ld] \n", p[i], sizeof(p[i]), strlen(p[i]));
  }
  free_2d(p, 20);

}

TEST(auto_buff, test) {
  auto ptr = create_auto_buf<int>(10, 1);

  REPL(i, 0, 10) {
    ptr.get()[i] = i;
    cout << ptr.get()[i] << " \n";
  }

}

TEST(file_exist, test) {
  cout << exists_file("/tmp") << "\n";
}

TEST(endswith, test) {
  cout << endswith("/tmp.jpg", ".txt") << "\n";
}

#include "lib/mat.h"
TEST(mat, test) {
  Mat<int> mat(2, 2, 2);
  for (int i = 0; i < mat.rows(); ++i) {
    for (int j = 0; j < mat.cols(); ++j) {
      for (int k = 0; k < mat.channels(); ++k) {
        mat.at(i, j, k) = 3 * i + 2 * j + k;
        cout << mat.at(i, j, k) << " ";
      }
      cout << "\n";
    }
    cout << "\n";
  }
  cout << *(mat.ptr(1, 1) + 1) << "\n";
  cout << *(mat.ptr() + 0) << " "
       << *(mat.ptr() + 1) << " ";

}

#include "lib/debugutils.h"
TEST(debug_print, test) {
//  P(1);
//  PP(2);
//  vector<int > a {1,2,3};
//  PA(a);

//  __m_assert_check__(false, "p1", __FILE__, __FUNCTION__, __LINE__ );
//  error_exit("p1");

  print_debug("p2 %d", 3);
}

#include "lib/matrix.h"
#include <Eigen/Dense>
TEST(mat_x, test) {
  typedef Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > _mat;

  Mat<double> m1_n(2, 2, 2);
  for (int i = 0; i < m1_n.channels(); ++i) {
    for (int j = 0; j < m1_n.rows(); ++j) {
      for (int k = 0; k < m1_n.cols(); ++k) {
        m1_n.at(j, k, i) = 3 * i + 2 * j + k;
        cout << "[" << j << k << i << "]" << m1_n.at(j, k, i) << " ";
      }
      cout << "\n";
    }
    cout << "\n";
  }

  Matrix m1(m1_n);
//  REPL(i, 0, 2) REPL(j, 0, 2) REPL(k, 0, 2) {
//        m1.at(i, j, k) = i * 2 + j * 1 + k;
//        cout << m1.at(i,j,k);
//      }

  cout << m1 << "\n";
  PP(m1.at(0, 0));
  PP(m1.at(0, 1));
  PP(m1.at(1, 0));
  PP(m1.at(1, 1));

  PP(m1.at(0, 0, 0));
  PP(m1.at(0, 1, 0));
  PP(m1.at(1, 0, 0));
  PP(m1.at(1, 1, 0));

  //  1. transpose
  auto m2 = m1.transpose();
  cout << "transpose: " << m2 << "\n";

  //  2. prod
  auto m1x2 = m1.prod(m2);
  cout << "prod: " << m1x2 << "\n";

  auto m3 = m1x2;
  //  3. elem_prod
  auto m23 = m2.elem_prod(m3);
  cout << "elem_prod: " << m23 << "\n";

  //  4. inverse
  auto m4 = m23;
  Matrix m5;
  auto ret = m4.inverse(m5);
  cout << "inverse: " << m5 << "\n";
  auto m4x5 = m4.prod(m5);
  cout << "inverse: " << m4x5 << "\n";

  //  5.  pseudo_inverse

  auto m6 = m4.pseudo_inverse();
  cout << "pseudo_inverse: " << m6 << "\n";
  auto m4x6 = m4.prod(m6);
  auto m4x6x4 = m4x6.prod(m4);
  cout << "pseudo_inverse: " << m4x6x4 << "\n";

}

TEST(normrot_, test) {
  Matrix m(3, 3);
  m.zero();
  cout << m << "\n";
}

TEST(sqr_sum, test) {
  Mat<double> m1_n(2, 2, 2);
  for (int i = 0; i < m1_n.channels(); ++i) {
    for (int j = 0; j < m1_n.rows(); ++j) {
      for (int k = 0; k < m1_n.cols(); ++k) {
        m1_n.at(j, k, i) = 3 * i + 2 * j + k;
        cout << "[" << j << k << i << "]" << m1_n.at(j, k, i) << " ";
      }
      cout << "\n";
    }
    cout << "\n";
  }

  Matrix m(m1_n);
  cout << m << "\n";

  auto sqr_sum = m.sqrsum();
  cout << sqr_sum << "\n";
  auto col = m.col(0);
  cout << col << "\n";
}

#include "lib/timer.h"
TEST(Timer_test, test) {
  Timer timer;
  sleep(1);
  auto t = timer.duration();
  cout << t << "\n";
}

TEST(GuardedTimer_test, test) {
  GuardedTimer timer("zzz");
  auto t = timer.duration();
  cout << t << "\n";
}

TEST(TotalTimer, test) {
  {
    TotalTimer timer(__func__);
//    sleep(1);
    timer.print();
  }
  sleep(1);
//  GUARDED_FUNC_TIMER;
}

#include "lib/geometry.h"
TEST(geometry, test) {
  Geometry geo(5, 2);
  cout << geo.area() << "\n";
  cout << geo.contain(3, 1) << "\n";
  cout << geo.ratio() << "\n";
}

TEST(vector, test) {
  Vector<int> vec(1, 2, 3);
  cout << vec.index(1) << "\n";

  cout << "min: " << vec.min_comp_abs() << "\n";
  cout << "sqr: " << vec.sqr() << "\n";

}

//  VectorTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class VectorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    vector_ = new Vector<double>(1, 2, 3);
    vector_1_ = new Vector<double>(4, -2, 3);
    vector_2_ = new Vector<double>(3, 1, 3);

  }

  virtual void TearDown() {
    delete vector_;
    delete vector_1_;
    delete vector_2_;
  }

  Vector<double> *vector_;
  Vector<double> *vector_1_;
  Vector<double> *vector_2_;

};

TEST_F(VectorTest, index) {
  EXPECT_EQ(1, vector_->index(0));
  EXPECT_EQ(2, vector_->index(1));
  EXPECT_EQ(3, vector_->index(2));
}

TEST_F(VectorTest, min_comp_abs) {
  EXPECT_EQ(2, vector_1_->min_comp_abs());
  cout << *vector_1_ << "\n";
}

TEST_F(VectorTest, sqr) {
  EXPECT_EQ(24, vector_1_->sqr());
}

TEST_F(VectorTest, mod) {
  EXPECT_DOUBLE_EQ(sqrt(16 + 4 + 9), vector_1_->mod());
}

TEST_F(VectorTest, dot) {
  EXPECT_DOUBLE_EQ(vector_->dot(*vector_1_), 9);
}

TEST_F(VectorTest, cross) {
  auto v = vector_->cross(*vector_1_);
  EXPECT_DOUBLE_EQ(v.index(0), 12);
  EXPECT_DOUBLE_EQ(v.index(1), 9);
  EXPECT_DOUBLE_EQ(v.index(2), -10);
}

TEST_F(VectorTest, normalize) {
  auto v = vector_->get_normalized();
  cout << v << "\n";
}

TEST_F(VectorTest, is_zero) {
  EXPECT_EQ(vector_->is_zero(), 0);
  auto v = vector_->get_zero();
  EXPECT_EQ(v.is_zero(), 1);
  cout << v << "\n";
}

TEST_F(VectorTest, max) {
  auto v = vector_->max();
  cout << v << "\n";
}

TEST_F(VectorTest, infinity) {
  auto v = vector_->infinity();
  cout << v << "\n";
}

TEST_F(VectorTest, get_max_min) {
  EXPECT_EQ(vector_->get_max(), 3);
  EXPECT_EQ(vector_->get_min(), 1);

  auto v = Vector<double>(-12, 1, 2);
  EXPECT_EQ(v.get_abs_max(), 12);

}

TEST_F(VectorTest, write_to) {
  auto v = Vector<double>(0, 1, 2);
  std::array<double, 3> arr{};
  v.write_to(arr.data());
  cout << v <<"\n";
  REP(i,3)  EXPECT_EQ(i, arr.at(i));
}
//  VectorTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//  Vector2DTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Vector2DTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    vec_   = new Vector2D<double>(1, 2);
    vec_1_ = new Vector2D<double>(4, -2);
    vec_2_ = new Vector2D<double>(3, 1);
  }

  virtual void TearDown() {
    delete vec_;
    delete vec_1_;
    delete vec_2_;
  }

  Vector2D<double> *vec_;
  Vector2D<double> *vec_1_;
  Vector2D<double> *vec_2_;

};

TEST_F(Vector2DTest, operator_){
  //  a = b + c
  auto vec = *vec_ + *vec_1_;
  EXPECT_EQ(vec.x, 5);
  EXPECT_EQ(vec.y, 0);

  //  a += b
  vec+=*vec_;
  EXPECT_EQ(vec.x, 6);
  EXPECT_EQ(vec.y, 2);

  //  a =-a
  vec = -vec;
  EXPECT_EQ(vec.x, -6);
  EXPECT_EQ(vec.y, -2);

  //  a = b - c
  vec = *vec_ - *vec_1_;
  EXPECT_EQ(vec.x, 1-4);
  EXPECT_EQ(vec.y, 2-(-2));

  //  a -= b
  vec-=*vec_;
  EXPECT_EQ(vec.x, -4);
  EXPECT_EQ(vec.y, 2);

  //  a = b * c
  vec = *vec_ * *vec_1_;
  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, -4);

  //  a *= T
  vec *= (2);
  EXPECT_EQ(vec.x, 8);
  EXPECT_EQ(vec.y, -8);

  //  a = a / T
  vec = vec / 2;
  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, -4);

  //  a = a * b
  vec = vec * *vec_;
  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, -8);

  //  a = a / b
  vec = vec / *vec_;
  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, -4);

  //  a == b?
  EXPECT_EQ(vec == *vec_, 0);

  //  !a  ==> y = -y
  vec = !vec;
  EXPECT_EQ(vec.y, 4);

  //  ~a  ==> swap(x,y)
  vec.x = 1, vec.y = 2;
  vec = ~vec;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 1);


}

TEST_F(Vector2DTest, write_to) {
  auto v = Vector<double>(0, 1, 2);
  std::array<double, 3> arr{};
  v.write_to(arr.data());
  cout << v <<"\n";
  REP(i,3)  EXPECT_EQ(i, arr.at(i));
}

//  Vector2DTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//  ColorTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "lib/color.h"
class ColorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    r_  = new pano::Color(1, 0, 0);
    g_  = new pano::Color(0, 1, 0);
    b_  = new pano::Color(0, 0, 1);
    c_  = new pano::Color(255, 255, 255);

  }

  virtual void TearDown() {
    delete r_;
    delete g_;
    delete b_;
    delete c_;

  }

  pano::Color *r_;
  pano::Color *g_;
  pano::Color *b_;
  pano::Color *c_;

};

TEST_F(ColorTest, check){
  auto c = c_->get_normalized();
  cout << c << "\n";
}


//  ColorTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//  ImgprocTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "lib/imgproc.h"
#include "lib/imgio.h"

TEST(read, chck){
  auto f = pano::read_img("/home/bys/test.jpg");
  cout << "jpg: " << f.cols() << " " << f.rows() << "\n";

  auto c = pano::read_img("/home/bys/test.png");
  cout << "png:" << c.cols() << " " << c.rows() << "\n";

  //  hcat
  auto h_cat = pano::hconcat(std::move(std::list<Mat32f>{f,c}));
  cout << "h_cat: " << h_cat.cols() << " " << h_cat.rows() << "\n";
  auto h_cat_color = pano::interpolate(h_cat, 1,1190);
  cout << h_cat_color << "\n";
  pano::write_rgb("/home/bys/save.jpg", h_cat);

  auto h_crop = pano::crop(f);
  cout << "h_crop: w h " << h_crop.width() << " " << h_crop.height() << "\n";

  pano::write_rgb("/home/bys/crop.jpg", h_crop);

  //  TODO  double free or corruption (out)
  /*  w   h   pix
   * 3533 1200 4239600
   * */
  {
    cout << "h_cat: w h pix "
         << h_cat.width() << " "
         << h_cat.height() << " "
         << h_cat.pixels() << "\n";
    Mat32f h_grey = pano::rgb2grey(h_cat);

    pano::write_rgb("/home/bys/grey.jpg", h_grey);
    cout << "h_grey: w h pix "
         << h_grey.cols() << " "
         << h_grey.rows() << " "
        << h_grey.channels() << " "

        << h_grey.pixels() << "\n";
//    pano::fill(h_grey, pano::Color::BLUE);  // Mat32f pic channel = 1 , so。。。
  }

  auto fill = Mat32f(3,3,3);
  pano::fill(fill, pano::Color::BLUE);
  pano::write_rgb("/home/bys/fill.jpg", fill);

  auto itp = pano::interpolate(fill, 0,0);
  cout << itp << "\n";

  auto crop_ = pano::crop(fill);
  cout << "crop: w h " << crop_.width() << " " << crop_.height() << "\n";


}

TEST(f2uc, test){
  assert(0);
}
//  ImgprocTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//  polygonTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "lib/polygon.h"
TEST(polygon, test){
  vector<Vec2D> polygon;
  polygon.emplace_back(1,6);
  polygon.emplace_back(8,5);
  polygon.emplace_back(4,4);
  polygon.emplace_back(7,2);
  polygon.emplace_back(3,1);
  REP(i, polygon.size())
  cout << i << ": " << polygon.at(i) << "\n";

  auto pol_1 = pano::convex_hull(polygon);
  REP(i, pol_1.size())
  cout << i << ": " << pol_1.at(i) << "\n";

  cout << "area: " << pano::polygon_area(polygon) << "\n";

}

TEST(PointInPolygonTest, test){
  vector<Vec2D> polygon;
  polygon.emplace_back(0,0);
  polygon.emplace_back(0,4);
  polygon.emplace_back(4,4);
  polygon.emplace_back(4,0);

  pano::PointInPolygon point_in_polygon(polygon);
  Vec2D vec_2_d(0,0);
  cout << point_in_polygon.in_polygon( vec_2_d ) << "\n";

  Vec2D vec_2_d_1(2,2);
  cout << point_in_polygon.in_polygon( vec_2_d_1 ) << "\n";

  Vec2D vec_2_d_2(-1,0);
  cout << point_in_polygon.in_polygon( vec_2_d_2 ) << "\n";
}

//  polygonTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//  PlanedrawerTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "lib/planedrawer.h"
TEST(PlanedrawerTest, test){

  Mat32f mat_32_f = pano::read_img("/home/bys/crop.jpg");
  pano::PlaneDrawer drawer(mat_32_f);
  drawer.set_rand_color();
  drawer.point(1,1);

  Vec2D p1(1,1);
  Vec2D p2(100,100);

  drawer.line(p1,p2);

  pano::write_rgb("/home/bys/draw.jpg", mat_32_f);
}
//  PlanedrawerTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//  polygonTest Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//  polygonTest End    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<