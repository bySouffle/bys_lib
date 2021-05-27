//
// Created by ubuntu on 5/24/21.
//

#include "gtest/gtest.h"
#include "StringOpt.h"

class StringOptTest : public ::testing::Test{
protected:
    virtual void SetUp(){
        stringopt = new StringOpt();
    }

    virtual void TearDown(){
        delete stringopt;
    }

    StringOpt *stringopt;

};

TEST_F(StringOptTest, str16tohex){  //"16"  ->  0x16
    long res = stringopt->str16tohex("0x16");
    EXPECT_EQ(res, 0x16);
    res = stringopt->str16tohex("0x00");
    EXPECT_EQ(res, 0x00);
    res = stringopt->str16tohex("0x1F");
    EXPECT_EQ(res, 0x1F);
    res = stringopt->str16tohex("0x7r");
    EXPECT_EQ(res, 7);
    res = stringopt->str16tohex("0xFFFFFFFF");
    EXPECT_EQ(res, 0xffffffff);
}

TEST_F(StringOptTest, hextostr16){  //  0x1F ->  "1F"
    char str[4] = {};
    stringopt->hextostr16(0x6F, str);
    EXPECT_STREQ(str,"6F");
    stringopt->hextostr16(0x12, str);
    EXPECT_STREQ(str,"12");
    stringopt->hextostr16(0x00, str);
    EXPECT_STREQ(str,"00");
    stringopt->hextostr16(0x6F, str);
    EXPECT_STREQ(str,"6F");
    stringopt->hextostr16('1', str);
    EXPECT_STREQ(str,"31");

}

TEST_F(StringOptTest, hextostr16_arr){  //  0x1F ->  "1F"
    char str[8] = {0x64,0x65,0x66,0x67,0x68,0x69,0x70,0x71};
    char sstr[48] = {};
    stringopt->hextostr16_arr(str, sstr);
    EXPECT_STREQ(sstr,"6465666768697071");

    memset(str, 0, sizeof (str));
    memcpy(str,"0123456",7);
    stringopt->hextostr16_arr(str, sstr);
    EXPECT_STREQ(sstr,"30313233343536");

    stringopt->hextostr16_arr("0", sstr);
    EXPECT_STREQ(sstr,"30");



}