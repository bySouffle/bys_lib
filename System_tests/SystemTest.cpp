//
// Created by ubuntu on 6/2/21.
//

#include "gtest/gtest.h"
#include "SystemOpt.h"

class SystemTest : public :: testing::Test {
protected:
    virtual void SetUp() {
        systemopt = new SystemOpt();
    }
    virtual void TearD() {
        delete systemopt;
    }

    SystemOpt* systemopt;
};

TEST_F(SystemTest, test_big_little_endian){     // pc little endian
    int endian = systemopt->test_big_little_endian();
    EXPECT_EQ(endian, 0);

}