//
// Created by bys on 2022/3/24.
//

#ifndef BYS_LIB_QT_LIB_BYS_QT_IRC_H_
#define BYS_LIB_QT_LIB_BYS_QT_IRC_H_

#include <string>
#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QFont>
//#include <QApplication>
//#include <QGuiApplication>


class IrcDraw {
 public:
  IrcDraw();
  IrcDraw(const std::string& pic_url);
 ~IrcDraw();
  int mark_max_temperature(const std::string&src, const std::string&dst,
                           unsigned short x, unsigned short y, double max_temperature);

  int draw_rect(unsigned short x, unsigned short  y);
  int put_text(const std::string& text);

  void pic_show();

 private:
  QImage img;
  QPainter *painter;
};

#endif //BYS_LIB_QT_LIB_BYS_QT_IRC_H_
