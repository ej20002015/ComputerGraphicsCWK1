#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <math.h>
#include "pixelwidget.hpp"

void PixelWidget::DrawLine(float x1, float y1, float x2, float y2)
{
  float dx = x2 - x1;
  float dy = y2 - y1;
  float absDx = fabs(dx);
  float absDy = fabs(dy);

  int dxSign = (dx > 0) ? 1 : -1;
  int dySign = (dy > 0) ? 1 : -1;

  int x = x1, y = y1;
  SetPixel(x, y, {255, 255, 255});
  for (int ix = 0, iy = 0; ix < absDx || iy < absDy;)
  {
    //Are we going to go up, down or diagonally
    float decision = (1 + 2 * ix) * absDy - (1 + 2 * iy) * absDx;

    if (!decision)
    {
      x += dxSign;
      y += dySign;
      ix++;
      iy++;
    }
    else if (decision < 0)
    {
      x += dxSign;
      ix++;
    }
    else
    {
      y += dySign;
      iy++;
    }
    
    SetPixel(x, y, {255, 255, 255});
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
  //SetPixel(14, 1, {255, 0, 0});
  //SetPixel(50, 60, {255, 0, 0});
  //DrawLine(14.0f, 1.0f, 50.0f, 60.0f);
  SetPixel(2, 50, {255, 0, 0});
  SetPixel(50, 2, {255, 0, 0});
  DrawLine(2.99f, 50.0f, 50.0f, 2.99f);
  //DrawLine(0.0f, 0.0f, 30.0f, 30.0f);

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
  p.drawLine(2, 50, 50, 2);
}
