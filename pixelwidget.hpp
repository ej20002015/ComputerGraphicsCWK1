#ifndef _PIXEL_WIDGET_
#define _PIXEL_WIDGET_

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <vector>
#include "RGBVal.hpp"
#include "Vec2.hpp"


class PixelWidget : public QWidget {
public:

  
  // set the number of pixels that the widget is meant to display
  PixelWidget
  (
   unsigned int n_horizontal, // the first integer determines the number of horizontal pixels 
   unsigned int n_vertical    // the second integer determines the number of vertical pixels
   );

  // sets a pixel at the specified RGBVal value; ignores non-existing pixels without warning
  void SetPixel
  ( 
   unsigned int  i_x, // horizontal pixel coordinate
   unsigned int  i_y, // vertical pixel coordinate
   const RGBVal& c    // RBGVal object for RGB values
    );

  // Use the body of this function to experiment with rendering algorithms
  void DefinePixelValues();

  /*
  \brief Rasterise a line on the screen from point1 to point2, interpolating the colour at each pixel. 
  Every pixel that is touched by the line will be set
  \param point1 Cartesian coordinate of the first point of the line
  \param point2 Cartesian coordinate of the second point of the line
  \param colour1 RGB value of the first point of the line
  \param colour2 RGB value of the second point of the line
  */
  void DrawLine(const Vec2<float>& point1, const Vec2<float>& point2, const RGBVal& colour1, const RGBVal& colour2);

  /*
  \brief Rasterise a triangle on the screen using barycentric coordinates, interpolating the colour of each pixel
  \param point1 Cartesian coordinates of the first point of the triangle
  \param point2 Cartesian coordinates of the second point of the triangle
  \param point3 Cartesian coordinates of the third point of the triangle
  \param colour1 RGB value of the first point of the triangle
  \param colour2 RGB value of the second point of the triangle
  \param colour3 RGB value of the third point of the triangle
  */
  void DrawTriangle(const Vec2<float>& point1, const Vec2<float>& point2, const Vec2<float>& point3,
                    const RGBVal& colour1, const RGBVal& colour2, const RGBVal& colour3);

  /*
  \brief Uses the half plane test to determine whether a point is inside or outside the triangle
  \param pixelCoordinatesPoint Pixel coordinates of the point to be tested
  \param trianglePoint1 Cartesian coordinates of the first point of the triangle
  \param trianglePoint2 Cartesian coordinates of the second point of the triangle
  \param trianglePoint3 Cartesian coordinates of the third point of the triangle
  \return If point is inside the triangle: true; If point is outside the triangle: false
  */
  bool IsInside(const Vec2<int>& pixelCoordinatesPoint, const Vec2<float>& trianglePoint1, const Vec2<float>& trianglePoint2,
                const Vec2<float>& trianglePoint3);

  /*
  \brief Writes a CSV list of barycentric coordinates for each pixel of the screen along with whether it is inside the triangle, 
  to the specified file
  \param filepath Filepath of the CSV file to write to. Will create the file if it doesn't exist.
  \param trianglePoint1 Cartesian coordinates of the first point of the triangle
  \param trianglePoint2 Cartesian coordinates of the second point of the triangle
  \param trianglePoint3 Cartesian coordinates of the third point of the triangle
  */
  void writeCoordinatesToFile(const std::string& filepath, const Vec2<float>& trianglePoint1, const Vec2<float>& trianglePoint2,
             const Vec2<float>& trianglePoint3);

  /*
  \brief Writes the current state of the framebuffer to a file using the PPM format
  \param filepath Filepath of the PPM file to write to. Will create the file if it doesn't exist.
  */
 void writeToPPMFile(const std::string& filepath);

protected:

  virtual void paintEvent(QPaintEvent*);

private:

  /*
  \brief Finds integers in the range [x, y] inclusive
  \param x Value at the start of the range to consider
  \param y Value at the end of the range to consider
  \return A list of integers that lie within the range [x, y]
  */
  std::vector<int> findIntegersInFloatRange(float x, float y);

  /*
  \brief Implements the half plane test to determine if a point is 'above' the line
  \param pixelCoordinatesPoint Pixel coordinates of the point to be tested
  \param point1 Cartesian coordinates of the first point of the line
  \param point2 Cartesian coordinate of the second point of the line
  \return If point is above the line: true; If not: false
  */
  bool IsAbove(const Vec2<int>& pixelCoordinatesPoint, const Vec2<float>& point1, const Vec2<float>& point2);


private:

  unsigned int _n_vertical;
  unsigned int _n_horizontal;

  std::vector<std::vector<RGBVal> > _vec_rects;
};

#endif

