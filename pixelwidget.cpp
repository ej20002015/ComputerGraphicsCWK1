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

#define LINE { 0.0, 0.0 }, { 5.0, 10.0 }

#define TRIANGLEP1 { 20.0, 3.0f }
#define TRIANGLEP2 { 10.0f, 42.0f }
#define TRIANGLEP3 { 50.0f, 64.0f }

void PixelWidget::DrawLine(const Vec2<float>& point1, const Vec2<float>& point2, const RGBVal& colour1, const RGBVal& colour2)
{
  //Calculate the x and y integer lines that are intersected

  std::vector<int> xIntercepts = findIntegersInFloatRange(point1.x, point2.x);
  std::vector<int> yIntercepts = findIntegersInFloatRange(point1.y, point2.y);

  //Only want to consider the positive and zero intersects
  //Would use std::remove_if but the compiler is ancient

  std::vector<int>::iterator it = xIntercepts.begin();
  while (it != xIntercepts.end())
  {
    int xIntercept = *it;
    if (xIntercept < 0 || xIntercept > static_cast<int>(this->_n_horizontal))
      it = xIntercepts.erase(it);
    else
      it++;
  }

  it = yIntercepts.begin();
  while (it != yIntercepts.end())
  {
    int yIntercept = *it;
    if (yIntercept < 0 || yIntercept > static_cast<int>(this->_n_vertical))
      it = yIntercepts.erase(it);
    else
      it++;
  }

  /* xIntercepts.erase(std::remove_if(xIntercepts.begin(), xIntercepts.end(), [this](int v) -> bool {
    return v < 0 || v > static_cast<int>(this->_n_horizontal);
  }), xIntercepts.end());
  yIntercepts.erase(std::remove_if(yIntercepts.begin(), yIntercepts.end(), [this](int v) -> bool { 
    return v < 0 || v > static_cast<int>(this-> _n_vertical); 
  }), yIntercepts.end()); */

  //Get t values of the line where it intersects the x and y integer lines
  
  std::vector<float> tValues;

  //Add the start and end t values to the vector since these need to be considered
  //when calculating midpoints of line segments
  tValues.push_back(0.0f);
  tValues.push_back(1.0f);

  if (point2.x - point1.x)
  {
    for (int i = 0; i < static_cast<int>(xIntercepts.size()); i++)
    {
      float x = static_cast<float>(xIntercepts[i]);
      tValues.push_back((x - point1.x) / (point2.x - point1.x));
    }
  }
  
  if (point2.y - point1.y)
  {
    for (int i = 0; i < static_cast<int>(yIntercepts.size()); i++)
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
  for (int i = 0; i < static_cast<int>(tValues.size()) - 1; i++)
    tMidpoints.push_back((tValues[i] + tValues[i + 1]) / 2.0f);


  for (int i = 0; i < static_cast<int>(tMidpoints.size()); i++)
  {
    //Substitute the midpoint values of t back into the equation of the line to get the x and y coordinates of the pixel to set 
    float t = tMidpoints[i];
    Vec2<float> intersectionPoint =
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
    SetPixel(static_cast<int>(intersectionPoint.x), static_cast<int>(intersectionPoint.y), interpolatedColour);
  }
}

void PixelWidget::DrawTriangle(const Vec2<float>& point1, const Vec2<float>& point2, const Vec2<float>& point3,
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
  int minX = static_cast<int>(std::min(point1.x, std::min(point2.x, point3.x)));
  int maxX = static_cast<int>(std::max(point1.x, std::max(point2.x, point3.x)));

  int minY = static_cast<int>(std::min(point1.y, std::min(point2.y, point3.y)));
  int maxY = static_cast<int>(std::max(point1.y, std::max(point2.y, point3.y)));

  std::vector<Vec2<int> > pixelCoords;
  std::vector<BarycentricCoordinates> baryCoords;

  for (int x = minX; x <= maxX && x < static_cast<int>(_n_horizontal); x++)
  {
    for (int y = minY; y <= maxY && x < static_cast<int>(_n_vertical); y++)
    {
      Vec2<float> cartesianCoords = { static_cast<float>(x), static_cast<float>(y) };
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

  for (int i = 0; i < static_cast<int>(baryCoords.size()); i++)
  {
    BarycentricCoordinates baryCoord = baryCoords[i];
    Vec2<int> pixelCoord = pixelCoords[i];

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

bool PixelWidget::IsInside(const Vec2<int>& pixelCoordinatesPoint, const Vec2<float>& trianglePoint1, const Vec2<float>& trianglePoint2, const Vec2<float>& trianglePoint3)
{
  //Determine whether the pixel coordinate is 'above' (same side as the normal pointing inwards) all 3 lines of the triangle
  //If so, it is in the triangle

  bool aboveLine1 = IsAbove(pixelCoordinatesPoint, trianglePoint1, trianglePoint2);
  bool aboveLine2 = IsAbove(pixelCoordinatesPoint, trianglePoint2, trianglePoint3);
  bool aboveLine3 = IsAbove(pixelCoordinatesPoint, trianglePoint3, trianglePoint1);

  if (aboveLine1 && aboveLine2 && aboveLine3)
    return true;
  else
    return false;
}

bool PixelWidget::IsAbove(const Vec2<int>& pixelCoordinatesPoint, const Vec2<float>& point1, const Vec2<float>& point2)
{
  Vec2<float> directionVector = point1 - point2;
  Vec2<float> lineNormal = directionVector.perp();
  Vec2<float> pointToTest = { static_cast<float>(pixelCoordinatesPoint.x), static_cast<float>(pixelCoordinatesPoint.y) };

  //Use the normal form of the line to determine if the point is above the line

  if ((pointToTest - point1).dot(lineNormal) > 0.0f)
    return true;
  else
    return false;
}

void PixelWidget::writeCoordinatesToFile(const std::string& filepath, const Vec2<float>& trianglePoint1, const Vec2<float>& trianglePoint2, const Vec2<float>& trianglePoint3)
{
  std::ofstream outputFile(filepath.c_str());

  //Write CSV headers
  outputFile << "Pixel Coordinates,Barycentric Coordinates Alpha,Barycentric Coordinates Beta,Barycentric Coordinates Gamma,Is Inside?" << std::endl;

  //Iterate over all pixel of the screen
  for (int x = 0; x < static_cast<int>(_n_horizontal); x++)
  {
    for (int y = 0; y < static_cast<int>(_n_vertical); y++)
    {
      Vec2<float> pixelCoordinateFloat = { static_cast<float>(x), static_cast<float>(y) };
      Vec2<int> pixelCoordinate = { x, y };
      BarycentricCoordinates baryCoordinate(pixelCoordinateFloat, trianglePoint1, trianglePoint2, trianglePoint3);
      bool isInside = IsInside(pixelCoordinate, trianglePoint1, trianglePoint2, trianglePoint3);
      
      outputFile << "\"(" << pixelCoordinate.x << ", " << pixelCoordinate.y << ")\",";
      outputFile << baryCoordinate.alpha << ",";
      outputFile << baryCoordinate.beta << ",";
      outputFile << baryCoordinate.gamma << ",";
      outputFile << isInside << std::endl;
    }
  }

  outputFile.close();
}

void PixelWidget::writeToPPMFile(const std::string& filepath)
{
  std::ofstream outputFile(filepath.c_str());

  //Write PPM Header
  outputFile << "P3 " << _n_horizontal << " " << _n_vertical << " " << 255 << std::endl;

  //Iterate over all pixels in the framebuffer, outputting the RGB values of each one
  for (int y = 0; y < static_cast<int>(_n_vertical); y++)
  {
    for (int x = 0; x < static_cast<int>(_n_horizontal); x++)
    {
      //Every 4 pixels, go onto a new line in the file to ensure the 70 character limit per line of PPM is not violated
      if ((x + y) % 4 == 0 && (x + y))
        outputFile << std::endl;
      
      outputFile << _vec_rects[x][y]._red << " ";
      outputFile << _vec_rects[x][y]._green << " ";
      outputFile << _vec_rects[x][y]._blue << "  ";
    }
  }

  outputFile.close();
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

  //TODO: consider where this code is suppose to be

  //DrawLinePerfect(LINE);
  //DrawLine(LINE, { 255, 255, 255 }, { 255, 255, 0 });

  Vec2<float> trianglePoint1 = TRIANGLEP1;
  RGBVal triangleColour1 = RGBVal{ 255, 0, 0 };
  Vec2<float> trianglePoint2 = TRIANGLEP2;
  RGBVal triangleColour2 = RGBVal{ 0, 255, 0 };
  Vec2<float> trianglePoint3 = TRIANGLEP3;
  RGBVal triangleColour3 = RGBVal{ 0, 0, 255 };
  //DrawTriangle(trianglePoint1, trianglePoint2, trianglePoint3, triangleColour1, triangleColour2, triangleColour3);

  //writeCoordinatesToFile("points.csv", TRIANGLEP1, TRIANGLEP2, TRIANGLEP3);

  DrawLine({ 1.0f, 50.0f }, { 50.0f, 1.0f }, RGBVal{ 255, 255, 255 }, RGBVal{ 255, 255, 255 });

  //writeToPPMFile("image.ppm");

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