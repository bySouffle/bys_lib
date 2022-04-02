//
// created by bys on 2022/3/24.
//

#include "bys_cv_irc.h"
#include <utility>

using namespace cv;

BysCvIrc::BysCvIrc() : pic_name_(), pic_mat_() {

}

BysCvIrc::BysCvIrc(std::string pic) : pic_name_(std::move(pic)), pic_mat_() {
  pic_mat_ = imread(pic_name_, IMREAD_COLOR);
}
int BysCvIrc::draw_rect(unsigned short x, unsigned short y) {

  Point o;

  o.x = (x - 80)>0? x-80 : 0 ;
  o.x = (x - 80)<pic_mat_.cols? x-80 : pic_mat_.cols ;



  o.y = (y - 80)>0? y-80 : 0 ;
  o.y = (y - 80)<pic_mat_.rows? y-80 : pic_mat_.rows ;

  Rect rect(o.x, o.y, 80, 80);//左上坐标（x,y）和矩形的长(x)宽(y)

  cv::rectangle(pic_mat_, rect, Scalar(0, 0, 255),2, LINE_8,0);

  return 0;
}
int BysCvIrc::put_text(const std::string &text) {
  if(pic_mat_.empty()){
    return -1;
  }
  int font_face = cv::FONT_HERSHEY_COMPLEX;
  double font_scale = 0.8;
  int thickness = 2;
  int baseline;
  //  文本长度
  Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);

  std::cout << "width " << text_size.width << " height " << text_size.height << " baseline " << baseline;

  Point origin;
  origin.x = pic_mat_.cols - text_size.width ;
  origin.y = pic_mat_.rows - text_size.height ;

  putText(pic_mat_, text, origin, font_face, font_scale,
          cv::Scalar(0, 0, 255), thickness, LINE_8);

  return 0;
}
void BysCvIrc::pic_show() {
  imshow("image", pic_mat_);
  waitKey(0);

}

BysCvIrc::~BysCvIrc() {
}
int BysCvIrc::mark_max_temperature(const std::string &src,
                                   const std::string &dst,
                                   unsigned short x,
                                   unsigned short y,
                                   double max_temperature) {
  pic_mat_.release();
  pic_mat_ = imread(src);
  draw_rect(x, y);
  std::string text = "max temperature:" + std::to_string(max_temperature) + "C";
  put_text(text);
  imwrite(dst,pic_mat_);

  return 0;
}

