//
// Created by ubuntu on 6/2/21.
//
#include <limits.h>
#include "gtest/gtest.h"
#include "SystemOpt.h"
#include "io_units.h"

class SystemTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    systemopt = new SystemOpt();
  }

  virtual void TearDown() {
    delete systemopt;
  }

  SystemOpt *systemopt;
};

TEST_F(SystemTest, test_big_little_endian) {     // pc little endian
  int endian = systemopt->test_big_little_endian();
  EXPECT_EQ(endian, 0);

}

TEST_F(SystemTest, test_uid_euid) {
  uid_t uid = getuid();
  uid_t euid = geteuid();
  printf("uid[%d]  euid[%d]\n", uid, euid);

  uid_t guid = getgid();
  uid_t geuid = getegid();
  printf("gid[%d]  egid[%d]\n", uid, euid);

}

TEST_F(SystemTest, test_setid) {     //  将uid root切换为普通usr
  int ret_val = systemopt->switch_process_to_user(503, 503);
  printf("ret_val = [%d]\n", ret_val);
}

TEST_F(SystemTest, test_chroot_chdir) {
  char current_dir[1024] = {};
  char new_dir[1024] = {};

  getcwd(current_dir, 1024);
  printf("current:[%s]\n", current_dir);

  strcat(current_dir, "/test");
  memcpy(new_dir, current_dir, 1024);

  mkdir(current_dir, 0775);
  chdir(current_dir);

  getcwd(current_dir, 1024);
  printf("change current:[%s]\n", current_dir);

  chdir(new_dir);
  chroot(new_dir);

  chdir("./test");
  getcwd(current_dir, 1024);
  printf(" / change current:[%s]\n", current_dir);

  chdir("./");
  getcwd(current_dir, 1024);
  printf(" / change current:[%s]\n", current_dir);

}

TEST_F(SystemTest, test_daemonize) {    //  test daemon
  int ret_val = systemopt->daemonize();
  EXPECT_EQ(ret_val, 1);
}

TEST(macro_define, PRINTLN) {
  std::string s("zzz");
  PRINT_STRING(s.data());
  PRINT_HEX(16);
  int arr[] = {1, 2, 3, 4, 5};
  PRINT_ARRAY("%d", arr, 5);
  PRINT_INT_ARRAY_LN(arr, 5);

  PRINT_WCHAR('a');
}

TEST(macro_define, PRINT_CHAR) {
  PRINT_CHAR('a');
}

TEST(macro_define, PRINT_WCHAR) {
  PRINT_WCHAR(0x8086);
}

TEST(macro_define, PRINT_INT) {
  PRINT_INT(1024);
}

TEST(macro_define, PRINT_LONG) {
  PRINT_LONG(LONG_MAX);
}

TEST(macro_define, PRINT_LLONG) {
  PRINT_LONGLONG(LONG_LONG_MAX);
}

TEST(macro_define, PRINT_BINARY) {
  std::cout << std::hex << UINT32_MAX << "\n";
  PRINT_BINARY(7);
}

TEST(macro_define, PRINT_HEX) {
  PRINT_HEX(INT32_MAX);
}

TEST(macro_define, PRINT_BOOL) {
  PRINT_BOOL(-1);
}

TEST(macro_define, PRINT_DOUBLE) {
  PRINT_DOUBLE(3.14159);
}

TEST(macro_define, PRINT_STRING) {
  PRINT_STRING("zzzzz");
}

TEST(macro_define, PRINT_ARRAY) {
  int a[5] = {};
  PRINT_ARRAY("%d", a, 5);
}

TEST(macro_define, PRINT_INT_ARRAY_LN) {
  int a[5] = {1};
  PRINT_INT_ARRAY_LN(a, 5);
}

TEST(macro_define, PRINT_INT_ARRAY) {
  int a[5] = {1};
  PRINT_INT_ARRAY(a, 5);
}

TEST(macro_define, PRINT_CHAR_ARRAY) {
  char a[5]{'1', '2', '3', '4', '5'};
  PRINT_CHAR_ARRAY(a, 5);
}

TEST(macro_define, PRINT_DOUBLE_ARRAY) {
  double d[5]{1.1, 2.2, 3.3, 4.4, 5.5};
  PRINT_DOUBLE_ARRAY(d, 5);
}

#include <random>
TEST(suffle, test){
  int arr[100]={};
  std::cout << (sizeof(arr)/sizeof (int) ) << "\n";

  auto f = [](int *arr, int len){
    srand(time(NULL));
    std::cout << (sizeof(*arr)/sizeof (int)) << "\n";
    for (int i = 0; i < len; ++i) {
      *(arr+i) = rand()%100;
    }
  };
  f(arr,100);
  PRINT_INT_ARRAY_LN(arr,100);
}

