//
// Created by ubuntu on 5/24/21.
//

#include "gtest/gtest.h"
#include "FileOpt.h"

class FileOptTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        fileopt = new FileOpt();
    }

    virtual void TearDown() {
        delete fileopt;
    }

    FileOpt *fileopt;
};

TEST_F(FileOptTest, get_current_url2) {
    std::string url = fileopt->get_current_url();
    printf("%s\n", url.data());
    std::string u("/home/ubuntu/bys_/bys_lib/cmake-build-debug/File_tests/%213525/-/21");
    fileopt->judge_dir_exist_and_create(u);
    u.resize(u.length()-3);
    int exist = access(u.data(),F_OK);
    EXPECT_EQ(exist,0);

}

TEST_F(FileOptTest, get_current_url) {
    std::string url = fileopt->get_current_url();
    printf("%s\n", url.data());
    fileopt->judge_dir_exist_and_create("/home/ubuntu/bys_/bys_lib/cmake-build-debug/File_tests/ZZZ/A/5");
    int exist = access("/home/ubuntu/bys_/bys_lib/cmake-build-debug/File_tests/ZZZ/A",F_OK);
    EXPECT_EQ(exist,0);
}

