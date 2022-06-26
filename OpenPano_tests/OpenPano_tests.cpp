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
  for (int i = 0; i < mat.cols(); ++i) {
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
  cout  << *(mat.ptr() + 0) << " "
      << *(mat.ptr() + 1) << " ";

}

#include "lib/debugutils.h"
TEST(debug_print, test){
//  P(1);
//  PP(2);
//  vector<int > a {1,2,3};
//  PA(a);

//  __m_assert_check__(false, "p1", __FILE__, __FUNCTION__, __LINE__ );
//  error_exit("p1");

  print_debug("p2 %d", 3);
}