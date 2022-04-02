//
// Created by bys on 2022/3/24.
//

#include "gtest/gtest.h"
#include "bys_cv_irc.h"

TEST(draw_text, test){
  std::string url = "/opt/1.jpg";
  BysCvIrc irc(url);
  irc.put_text("max temperature:36C");
  irc.draw_rect(50,50);
  irc.pic_show();
}

TEST(save_deal_irc, test){
  BysCvIrc bys;
  std::string src = "/opt/1.jpg";
  std::string dst = "/opt/111zzz.jpg";

  bys.mark_max_temperature(src, dst, 1500, 1160, 37.2);
  bys.pic_show();
}