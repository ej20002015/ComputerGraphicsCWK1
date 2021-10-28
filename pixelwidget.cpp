#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include <math.h>
#include "pixelwidget.hpp"


void PixelWidget::DrawLine(float x1, float y1, float x2, float y2)
{
  // Line: x = (1 - t) * x1 + t * x2;
  //       y = (1 - t) * y1 + t * y2;

  float dx = x2 -x1;
  float dy = y2 - y1;

  //This is good
  int steps = dx == dy ? dx : dx + dy;

  float stepSize = 1.0f / steps;

  /* float lineDistance = sqrt(pow(x2 - x1 + 1.0f, 2.0f) + pow(y2 - y1 + 1.0f, 2.0f));
  float stepSize = 1.0f / lineDistance;
  stepSize /= (70); */
  std::cout << "steps: " << steps << std::endl;

  //float stepSize = 0.01f;
  for (unsigned int t = 0; t < steps + 1; t++)
  {
    float x = ((steps - t) * (1.0f / steps)) * x1 + (t * (1.0f / steps) * x2);
    float y = ((steps - t) * (1.0f / steps)) * y1 + (t * (1.0f / steps) * y2);
    //float y = (1 - t) * y1 + t * y2;
    std::cout << "FLOAT: (" << x << ", " << y << ")" << std::endl;
    std::cout << "UINT:  (" << static_cast<unsigned int>(x) << ", " << static_cast<unsigned int>(y) << ")" << std::endl;
    SetPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), {255, 255, 255});
  }

}


void PixelWidget::DefinePixelValues(){ //add pixels here; write methods like this for the assignments
}


// -----------------Most code below can remain untouched -------------------------
// ------but look at where DefinePixelValues is called in paintEvent--------------

PixelWidget::PixelWidget(unsigned int n_vertical, unsigned int n_horizontal):
  _n_vertical   (n_vertical),
  _n_horizontal (n_horizontal),
  _vec_rects(0)
{
  // all pixels are initialized to black
     for (unsigned int i_col = 0; i_col < n_vertical; i_col++)
       _vec_rects.push_back(std::vector<RGBVal>(n_horizontal));
}



void PixelWidget::SetPixel(unsigned int i_column, unsigned int i_row, const RGBVal& rgb)
{

  // if the pixel exists, set it
  if (i_column < _n_vertical && i_row < _n_horizontal)
    _vec_rects[i_column][i_row] = rgb;
}


void PixelWidget::paintEvent( QPaintEvent * )
{

  QPainter p( this );
  // no antialiasing, want thin lines between the cell
  p.setRenderHint( QPainter::Antialiasing, false );

  // set window/viewport so that the size fits the screen, within reason
  p.setWindow(QRect(-1.,-1.,_n_vertical+2,_n_horizontal+2));
  int side = qMin(width(), height());  
  p.setViewport(0, 0, side, side);

  // black thin boundary around the cells
  QPen pen( Qt::black );
  pen.setWidth(0.);

  // here the pixel values defined by the user are set in the pixel array
  DefinePixelValues();

  // Draw a line from point (0.0, 0.0) to (30.0f, 15.0f)
  DrawLine(10.0f, 10.0f, 23.0f, 69.0f);
  //SetPixel(22.0f, 68.0f, {255, 0, 0});

  for (unsigned int i_column = 0 ; i_column < _n_vertical; i_column++)
    for(unsigned int i_row = 0; i_row < _n_horizontal; i_row++){
      QRect rect(i_column,i_row,1,1);
      QColor c = QColor(_vec_rects[i_column][i_row]._red, _vec_rects[i_column][i_row]._green, _vec_rects[i_column][i_row]._blue);
    
      // fill with color for the pixel
      p.fillRect(rect, QBrush(c));
      p.setPen(pen);
      p.drawRect(rect);
    }

  //Draw line in Qt to check
  p.setPen(QPen(Qt::red, 0.1));
  p.drawLine(10, 10, 23, 69);
}
