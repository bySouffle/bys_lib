//
// Created by bys on 2022/3/25.
//

#include "gtest/gtest.h"
#include "macro_define.h"

TEST(testprint, test){
  int a = 10;
  PRINTLNF("%d",a);
  PRINT_INT(a);
}