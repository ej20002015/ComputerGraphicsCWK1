#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include "pixelwidget.hpp"


void PixelWidget::DrawLine(float x1, float y1, float x2, float y2)
{
  // Line: x = (1 - t) * x1 + t * x2;
  //       y = (1 - t) * y1 + t * y2;

  int dx = static_cast<int>(fabs(x2 - x1));
  int dy = static_cast<int>(fabs(y2 - y1));

  std::cout << "dx: " << dx << " dy: " << dy << std::endl;

  //This is good
  int steps = dx == dy ? dx : dx + dy;

  //steps = 100;

  if (!steps)
    return;

  float stepSize = 1.0f / steps;

  /* float lineDistance = sqrt(pow(x2 - x1 + 1.0f, 2.0f) + pow(y2 - y1 + 1.0f, 2.0f));
  float stepSize = 1.0f / lineDistance;
  stepSize /= (70); */
  std::cout << "steps: " << steps << std::endl;

  //float stepSize = 0.01f;
  for (unsigned int i = 0; i < steps + 1; i++)
  {
    float t = i * (1.0f / steps);
    printf("t: %.10f\n", t);

    float x = ((1 - t) * x1 + t * x2) + (stepSize / 2.0f);
    float y = ((1 - t) * y1 + t * y2) + (stepSize / 2.0f);
    //std::cout << "epsilon value: " << std::numeric_limits<float>::epsilon();
    //float y = (1 - t) * y1 + t * y2;
    printf("%.10f\n", x);
    printf("%.10f\n", y);
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
  //SetPixel(14, 1, {255, 0, 0});
  //SetPixel(50, 60, {255, 0, 0});
  //DrawLine(14.0f, 1.0f, 50.0f, 60.0f);
  SetPixel(3, 50, {255, 0, 0});
  SetPixel(50, 3, {255, 0, 0});
  DrawLine(3.0f, 50.0f, 50.0f, 3.0f);
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
  p.drawLine(14, 1, 50, 60);
}
