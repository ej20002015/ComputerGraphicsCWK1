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

#include "Vec2.hpp"
#include "BarycentricCoordinates.hpp"
#include "PixelCoordinates.hpp"

//#define LINE 2.999f, 49.999f, 49.999f, 2.999f
//#define LINE 14.0f, 1.0f, 50.0f, 60.0f
//#define LINE 0.5f, 0.5f, 2.5f, 2.5f
//#define LINE 
//#define LINE 0.0f, 69.0f, 69.0f, 0.0f

//TODO: Get rid of this stuff
template<typename T>
void printVector(const std::vector<T>& vec, const std::string& vectorName)
{
  std::cout << "\n" << vectorName << ": \n" << std::endl;
  for (int i = 0; i < vec.size(); i++)
    std::cout << vectorName << " value: " << vec[i] << std::endl;
}

//#define LINE 0.0f, 0.0f, 2.0f, 2.0f
//#define LINE 0.5f, 0.5f, 2.5f, 2.5f
//#define LINE 0.0f, 2.0f, 2.0f, 0.0f
//#define LINE 0.5f, 2.5f, 2.5f, 0.5f
//#define LINE 2.0f, 0.0f, 0.0f, 2.0f
//#define LINE 2.5f, 0.5f, 0.5f, 2.5f
//#define LINE 2.0f, 2.0f, 0.0f, 0.0f
//#define LINE 2.5f, 2.5f, 0.5f, 0.5f
//#define LINE -10.5, -10.5, 2.5f, 2.5f
//#define LINE 2.5, 2.5, 2.0f, 2.0f
//#define LINE 0.9, 0.9, 1.1f, 69.5f
//#define LINE 0.0, 0.0, 0.0, 75.0
#define LINE { 0.0, 0.0 }, { 5.0, 10.0 }

#define TRIANGLEP1 { 0.0f, 0.0f }
#define TRIANGLEP2 { 2.0f, 69.0f }
#define TRIANGLEP3 { 69.0f, 30.0f }

//Draw a line between points 1 and 2, lighting all pixels that are touched by it
//The top left of each pixel is (0,0) in pixel space
//The interpolated value (colour) of each pixel is scaled by the t value of the midpoint of the line
//segment that passes through the pixel
void PixelWidget::DrawLine(const Vec2& point1, const Vec2& point2, const RGBVal& colour1, const RGBVal& colour2)
{
  std::cout << "Drawing Line\n" << std::endl;

  //Calculate the x and y integer lines that are intersected

  std::vector<int> xIntercepts = findIntegersInFloatRange(point1.x, point2.x);
  std::vector<int> yIntercepts = findIntegersInFloatRange(point1.y, point2.y);

  //Only want to consider the positive and zero intersects

  xIntercepts.erase(std::remove_if(xIntercepts.begin(), xIntercepts.end(), [this](int v) -> bool {
    return v < 0 || v > this->_n_horizontal;
  }), xIntercepts.end());
  yIntercepts.erase(std::remove_if(yIntercepts.begin(), yIntercepts.end(), [this](int v) -> bool { 
    return v < 0 || v > this-> _n_vertical; 
  }), yIntercepts.end());

  //Get t values of the line where it intersects the x and y integer lines
  
  std::vector<float> tValues;

  //Add the start and end t values to the vector since these need to be considered
  //when calculating midpoints of line segments
  tValues.push_back(0.0f);
  tValues.push_back(1.0f);

  if (point2.x - point1.x)
  {
    for (int i = 0; i < xIntercepts.size(); i++)
    {
      float x = static_cast<float>(xIntercepts[i]);
      tValues.push_back((x - point1.x) / (point2.x - point1.x));
    }
  }
  
  if (point2.y - point1.y)
  {
    for (int i = 0; i < yIntercepts.size(); i++)
    {
      float y = static_cast<float>(yIntercepts[i]);
      tValues.push_back((y - point1.y) / (point2.y - point1.y));
    }
  }

  //Sort the vector of t values and then find the midpoints of the adjacent pairs.
  //This midpoint yields a t value that is the midpoint of the line segment that
  //passes through a pixel. These t values are better to use than those of the
  //intersections since we do not have to deal with floating point imprecision
  //when representing integers.

  std::sort(tValues.begin(), tValues.end());

  std::vector<float> tMidpoints;
  for (int i = 0; i < tValues.size() - 1; i++)
    tMidpoints.push_back((tValues[i] + tValues[i + 1]) / 2.0f);


  for (int i = 0; i < tMidpoints.size(); i++)
  {
    //Substitute the midpoint values of t back into the equation of the line to get the x and y coordinates of the pixel to set 
    float t = tMidpoints[i];
    Vec2 intersectionPoint =
    {
      (1 - t) * point1.x + t * point2.x,
      (1 - t) * point1.y + t * point2.y
    };

    //Calculate the interpolated colour at this point
    RGBVal interpolatedColour;
    interpolatedColour._red   = static_cast<unsigned int>((1 - t) * colour1._red + t * colour2._red);
    interpolatedColour._green = static_cast<unsigned int>((1 - t) * colour1._green + t * colour2._green);
    interpolatedColour._blue  = static_cast<unsigned int>((1 - t) * colour1._blue + t * colour2._blue);

    //Set the pixel
    std::cout << "Pixel: Coordinates(" << static_cast<int>(intersectionPoint.x) << ", " << static_cast<int>(intersectionPoint.y) << ") RGBColour(" <<
    interpolatedColour._red << ", " << interpolatedColour._green << ", " << interpolatedColour._blue << ")" << std::endl;
    SetPixel(static_cast<int>(intersectionPoint.x), static_cast<int>(intersectionPoint.y), interpolatedColour);
  }

  std::cout << "Drew line\n" << std::endl;
  
}

void PixelWidget::DrawLinePerfect(const Vec2& point1, const Vec2& point2, bool debugInfo)
{
  // Line: x = (1 - t) * x1 + t * x2;
  //       y = (1 - t) * y1 + t * y2;

  uint32_t steps = 10000000;

  for (unsigned int i = 0; i < steps + 1; i++)
  {
    float t = i * (1.0f / steps);

    float x = ((1 - t) * point1.x + t * point2.x);// + (stepSize / 2.0f);
    float y = ((1 - t) * point1.y + t * point2.y);// + (stepSize / 2.0f);
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

void PixelWidget::DrawTriangle(const Vec2& point1, const Vec2& point2, const Vec2& point3,
                  const RGBVal& colour1, const RGBVal& colour2, const RGBVal& colour3)
{
  //First draw the perimeter of the triange:
  DrawLine(point1, point2, colour1, colour2);
  DrawLine(point2, point3, colour2, colour3);
  DrawLine(point3, point1, colour3, colour1);

  //Now loop over pixels, and convert them to barycentric coordinates
  //(Since drawing the perimeter of the triangle will completely enclose all
  //the pixels inside the triangle, we can just sample the top-left corner of
  //every pixel (i.e. integer pixel coordinates can be used))

  //We could loop over all pixels but that is unecessary.
  //We only need to consider pixels that are between the min and max x-values of the points,
  //and those between the min and max y-values of the points
  int minX = static_cast<int>(std::min({ point1.x, point2.x, point3.x }));
  int maxX = static_cast<int>(std::max({ point1.x, point2.x, point3.x }));

  int minY = static_cast<int>(std::min({ point1.y, point2.y, point3.y }));
  int maxY = static_cast<int>(std::max({ point1.y, point2.y, point3.y }));

  std::vector<PixelCoordinates> pixelCoords;
  std::vector<BarycentricCoordinates> baryCoords;

  for (int x = minX; x <= maxX && x < static_cast<int>(_n_horizontal); x++)
  {
    for (int y = minY; y <= maxY && x < static_cast<int>(_n_vertical); y++)
    {
      Vec2 cartesianCoords = { static_cast<float>(x), static_cast<float>(y) };
      baryCoords.push_back(BarycentricCoordinates(cartesianCoords, point1, point2, point3));
      pixelCoords.push_back({ x, y });
    }
  }

  //Loop through each point in barycentric coordinates and set the pixel if it lies in the triangle,
  //that is, if the following conditions are satisfied:
  //
  // 0 < alpha < 1
  // 0 < beta < 1
  // 0 < gamma < 1
  // alpha + beta < 1

  for (int i = 0; i < baryCoords.size(); i++)
  {
    BarycentricCoordinates baryCoord = baryCoords[i];
    PixelCoordinates pixelCoord = pixelCoords[i];

    bool cond1 = baryCoord.alpha > 0.0f && baryCoord.alpha < 1.0f;
    bool cond2 = baryCoord.beta > 0.0f && baryCoord.beta < 1.0f;
    bool cond3 = baryCoord.gamma > 0.0f && baryCoord.gamma < 1.0f;
    bool cond4 = baryCoord.alpha + baryCoord.beta < 1.0f;

    if (cond1 && cond2 && cond3 && cond4)
    {
      //Calculate the interpolated colour at this point
      RGBVal interpolatedColour;
      interpolatedColour._red   = static_cast<unsigned int>(baryCoord.alpha * colour1._red + baryCoord.beta * colour2._red + baryCoord.gamma * colour3._red);
      interpolatedColour._green = static_cast<unsigned int>(baryCoord.alpha * colour1._green + baryCoord.beta * colour2._green + baryCoord.gamma * colour3._green);
      interpolatedColour._blue  = static_cast<unsigned int>(baryCoord.alpha * colour1._blue + baryCoord.beta * colour2._blue + baryCoord.gamma * colour3._blue);
  
      SetPixel(pixelCoord.x, pixelCoord.y, interpolatedColour);
    }
    
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
  //DrawLinePerfect(LINE);
  //DrawLine(LINE, { 255, 255, 255 }, { 255, 255, 0 });

  Vec2 trianglePoint1 = TRIANGLEP1;
  RGBVal triangleColour1 = { 255, 0, 0 };
  Vec2 trianglePoint2 = TRIANGLEP2;
  RGBVal triangleColour2 = { 0, 255, 0 };
  Vec2 trianglePoint3 = TRIANGLEP3;
  RGBVal triangleColour3 = { 0, 0, 255 };
  DrawTriangle(trianglePoint1, trianglePoint2, trianglePoint3, triangleColour1, triangleColour2, triangleColour3);

  DrawLine(trianglePoint1, trianglePoint2, { 255, 255, 255 }, { 255, 255, 255 });
  DrawLine(trianglePoint2, trianglePoint3, { 255, 255, 255 }, { 255, 255, 255 });
  DrawLine(trianglePoint3, trianglePoint1, { 255, 255, 255 }, { 255, 255, 255 });

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