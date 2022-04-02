//
// Created by bys on 2022/3/24.
//

#include "gtest/gtest.h"
#include "bys_qt_irc.h"
#include <QCoreApplication>
TEST(draw, test){

  auto f = [](int argc, char *argv[])->int{
    QCoreApplication app(argc, argv);
    std::string src = "/opt/1.jpg";
    std::string dst = "/opt/111zzz.jpg";
    IrcDraw irc(src);

    irc.mark_max_temperature(src, dst, 150, 150, 37.2);
    irc.pic_show();
    return app.exec();
  };


}