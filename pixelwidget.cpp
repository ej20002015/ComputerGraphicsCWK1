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
#include <algorithm>
#include "pixelwidget.hpp"

#define LINE 2.9f, 49.9f, 49.9f, 2.9f
//#define LINE 14.0f, 1.0f, 50.0f, 60.0f
//#define LINE 0.5f, 0.5f, 2.5f, 2.5f
//#define LINE 
//#define LINE 0.0f, 69.0f, 69.0f, 0.0f

/* #define LINE 0.0f, 0.0f, 2.0f, 2.0f
#define LINE 0.5f, 0.5f, 2.5f, 2.5f
#define LINE 0.0f, 2.0f, 2.0f, 0.0f
#define LINE 0.5f, 2.5f, 2.5f, 0.5f
#define LINE 2.0f, 0.0f, 0.0f, 2.0f
#define LINE 2.5f, 0.5f, 0.5f, 2.5f
#define LINE 2.0f, 2.0f, 0.0f, 0.0f
#define LINE 2.5f, 2.5f, 0.5f, 0.5f */
//#define LINE -10.5, -10.5, 2.5f, 2.5f
//#define LINE 2.5, 2.5, 2.0f, 2.0f

void PixelWidget::DrawLine(float x1, float y1, float x2, float y2, const RGBVal& colour1, const RGBVal& colour2)
{
  // Line: x = (1 - t) * x1 + t * x2;
  //       y = (1 - t) * y1 + t * y2;

  std::cout << "Drawing Line\n" << std::endl;

  //Calculate the x and y integer lines that are intersected

  std::vector<int> xIntercepts = findIntegersInFloatRange(x1, x2);
  std::vector<int> yIntercepts = findIntegersInFloatRange(y1, y2);

  //Only want to consider the positive and zero intersects

  xIntercepts.erase(std::remove_if(xIntercepts.begin(), xIntercepts.end(), [this](int v) -> bool {
    return v < 0 || v > this->_n_horizontal;
  }), xIntercepts.end());
  yIntercepts.erase(std::remove_if(yIntercepts.begin(), yIntercepts.end(), [this](int v) -> bool { 
    return v < 0 || v > this-> _n_vertical; 
  }), yIntercepts.end());

  //Get t values of the line where it intersects the x and y integer lines
  
  std::vector<float> tValues;

  for (int i = 0; i < xIntercepts.size(); i++)
  {
    ParameterisedLine drawLine = { { x1, y1 }, { x2, y2 } };
    ParameterisedLine verticalLine = { { static_cast<float>(xIntercepts[i]), 0.0f }, { static_cast<float>(xIntercepts[i]), static_cast<float>(_n_vertical) } };
    tValues.push_back(findIntersection(drawLine, verticalLine));
  }

  for (int i = 0; i < yIntercepts.size(); i++)
  {
    ParameterisedLine drawLine = { { x1, y1 }, { x2, y2 } };
    ParameterisedLine horizontalLine = { { 0.0f, static_cast<float>(yIntercepts[i]) }, { static_cast<float>(_n_vertical), static_cast<float>(yIntercepts[i]) } };
    tValues.push_back(findIntersection(drawLine, horizontalLine));
  }

  //Sort the vector of t values and then find the midpoints of the adjacent pairs.
  //This midpoint yields a t value that is the midpoint of the line segment that
  //passes through a pixel. These t values are better to use than those of the
  //intersections since we do not have to deal with floating point inprecision when
  //representing integers.

  std::sort(tValues.begin(), tValues.end());

  std::vector<float> tMidpoints;
  for (int i = 0; i < tValues.size() - 1; i++)
    tMidpoints.push_back((tValues[i] + tValues[i + 1]) / 2.0f);

  //Add the midpoint t values of the first and last line segments
  tMidpoints.push_back((0.0f + tValues.front()) / 2.0f);
  tMidpoints.push_back((1.0f + tValues.back()) / 2.0f);

  for (int i = 0; i < tMidpoints.size(); i++)
  {
    std::cout << "Midpoint value: " << tMidpoints[i] << std::endl;
    //Substitute the midpoint values of t back into the equation of the line to get the x and y coordinates of the pixel to set 
    ParameterisedLine drawLine = { { x1, y1 }, { x2, y2 } };
    float t = tMidpoints[i];
    vec2 intersectionPoint = ((1 - t) * drawLine.p + t * drawLine.q) + std::numeric_limits<float>::epsilon();

    //Calculate the interpolated colour at this point
    RGBVal interpolatedColour;
    interpolatedColour._red   = static_cast<unsigned int>(((1 - t) * colour1._red + t * colour2._red));
    interpolatedColour._green = static_cast<unsigned int>(((1 - t) * colour1._green + t * colour2._green));
    interpolatedColour._blue  = static_cast<unsigned int>(((1 - t) * colour1._blue + t * colour2._blue));

    //Set the pixel
    std::cout << "Pixel: (" << static_cast<int>(intersectionPoint.x) << ", " << static_cast<int>(intersectionPoint.y) << ")" << std::endl;
    SetPixel(static_cast<int>(intersectionPoint.x), static_cast<int>(intersectionPoint.y), interpolatedColour);
  }

  std::cout << "Drew line\n" << std::endl;
  
}

void PixelWidget::DrawLinePerfect(float x1, float y1, float x2, float y2, bool debugInfo)
{
  // Line: x = (1 - t) * x1 + t * x2;
  //       y = (1 - t) * y1 + t * y2;

  uint32_t steps = 10000;

  for (unsigned int i = 0; i < steps + 1; i++)
  {
    float t = i * (1.0f / steps);

    float x = ((1 - t) * x1 + t * x2);// + (stepSize / 2.0f);
    float y = ((1 - t) * y1 + t * y2);// + (stepSize / 2.0f);
    //std::cout << "epsilon value: " << std::numeric_limits<float>::epsilon();
    //float y = (1 - t) * y1 + t * y2;
    if (debugInfo)
    {
      printf("%.10f\n", x);
      printf("%.10f\n", y);
      std::cout << "UINT:  (" << static_cast<unsigned int>(x) << ", " << static_cast<unsigned int>(y) << ")" << std::endl;
    }
    SetPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), {0, 0, 255});
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
  //SetPixel(2, 50, {255, 0, 0});
  //SetPixel(50, 2, {255, 0, 0});
  DrawLinePerfect(LINE);
  DrawLine(LINE, { 255, 0, 255 }, { 255, 255, 0 });
  //DrawLine(0.0f, 0.0f, 30.0f, 30.0f);

  /* DrawLine(0.0f, 0.0f, 2.0f, 2.0f);
  DrawLine(0.5f, 0.5f, 2.5f, 2.5f);
  DrawLine(0.0f, 2.0f, 2.0f, 0.0f);
  DrawLine(0.5f, 2.5f, 2.5f, 0.5f);
  DrawLine(2.0f, 0.0f, 0.0f, 2.0f);
  DrawLine(2.5f, 0.5f, 0.5f, 2.5f);
  DrawLine(2.0f, 2.0f, 0.0f, 0.0f);
  DrawLine(2.5f, 2.5f, 0.5f, 0.5f); */

  for (unsigned int i_column = 0 ; i_column < _n_vertical; i_column++)
    for(unsigned int i_row = 0; i_row < _n_horizontal; i_row++){
      QRect rect(i_column,i_row,1,1);
      QColor c = QColor(_vec_rects[i_column][i_row]._red, _vec_rects[i_column][i_row]._green, _vec_rects[i_column][i_row]._blue);
    
      // fill with color for the pixel
      p.fillRect(rect, QBrush(c));
      p.setPen(pen);
      p.drawRect(rect);
    }
}

std::vector<int> PixelWidget::findIntegersInFloatRange(float v1, float v2)
{
  std::vector<int> integerIntercepts;

  float delta = v2 - v1;

  int sign = delta > 0 ? 1 : -1;

  int startingX = sign == 1 ? ceil(v1) : floor(v1);

  int x = startingX;
  for (int i = 0; i < fabs(delta); x += sign, i++)
    integerIntercepts.push_back(x);

  //If v2 is an integer then add to the list of intercepts
  if (static_cast<float>(x) == v2)
    integerIntercepts.push_back(x);

  return integerIntercepts;
}

float PixelWidget::findIntersection(const ParameterisedLine& line1, const ParameterisedLine& line2)
{
  vec2 u = line2.p - line2.q;
  vec2 v = line1.p - line1.q;
  float t = ((line2.q - line1.q).dot(u.perp())) / (v.dot(u.perp()));

  return t;
}
