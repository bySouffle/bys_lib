//
// Created by bys on 2022/3/24.
//

#ifndef BYS_LIB_BYS_CV_IRC_H
#define BYS_LIB_BYS_CV_IRC_H

#include "opencv2/opencv.hpp"

class BysCvIrc {
 public:
  BysCvIrc();
  BysCvIrc(std::string pic);
  ~ BysCvIrc();

  int mark_max_temperature(const std::string&src, const std::string&dst,
                           unsigned short x, unsigned short y, double max_temperature);

  int draw_rect(unsigned short x, unsigned short  y);
  int put_text(const std::string& text);

  void pic_show();
 private:
  std::string pic_name_;
  cv::Mat pic_mat_;
};

#endif //BYS_LIB_BYS_CV_IRC_H
