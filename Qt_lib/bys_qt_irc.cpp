//
// Created by bys on 2022/3/24.
//

#include "bys_qt_irc.h"
IrcDraw::IrcDraw() {

}
IrcDraw::IrcDraw(const std::string &pic_url) : img(pic_url.data()) {
//  QPixmap pixmap =  QPixmap::fromImage( img )  ;
  painter =  new QPainter ( &img ) ;
}
int IrcDraw::mark_max_temperature(const std::string &src,
                                  const std::string &dst,
                                  unsigned short x,
                                  unsigned short y,
                                  double max_temperature) {

  QPoint point ( x ,  y ) ;
  std::string text = "max temperature:" + std::to_string(max_temperature) + "C";

  draw_rect(x , y) ;
  put_text( text ) ;

  img.save(dst.data());

  return 0;
}
int IrcDraw::draw_rect(unsigned short x, unsigned short y) {

#define  gap_max 10

  QColor color  ;

  color = QColor ( 255 , 0 ,  0 ) ;

  painter->setPen(  color  ) ;

  painter->drawLine( x - gap_max , y - gap_max  ,  x - gap_max    , y - gap_max /2  ) ;
  painter->drawLine( x - gap_max , y - gap_max  ,  x - gap_max/2  , y - gap_max  ) ;

  painter->drawLine( x + gap_max , y - gap_max  ,  x + gap_max    , y - gap_max /2  ) ;
  painter->drawLine( x + gap_max , y - gap_max  ,  x + gap_max/2  , y - gap_max  ) ;

  painter->drawLine( x + gap_max , y + gap_max  ,  x + gap_max    , y + gap_max /2  ) ;
  painter->drawLine( x + gap_max , y + gap_max  ,  x + gap_max/2  , y + gap_max  ) ;

  painter->drawLine( x - gap_max , y + gap_max  ,  x - gap_max    , y + gap_max /2  ) ;
  painter->drawLine( x - gap_max , y + gap_max  ,  x - gap_max/2  , y + gap_max  ) ;

  return 0;
}
int IrcDraw::put_text(const std::string &text) {
//  QFont font = painter->font("宋体", 15, QFont::Bold, true) ;

  QFont font=painter->font();


  font.setBold( true ) ;
  painter->setFont( font  ) ;

  QColor color  ;

  color = QColor ( 128 , 128 ,  128 ) ;

  painter->setPen(  color  ) ;

  int y  = 10 ;
  int x  = 10 ;
  int w  = 200 ;
  int h  = 20 ;

  QRectF rectf ( x , y ,w ,h ) ;

  painter->drawText( rectf ,"z" ) ;
//  painter->drawText( 20,20 ,text.data() ) ;

  return 0;
}
void IrcDraw::pic_show() {

}
IrcDraw::~IrcDraw() {

  painter->end();
}


