#ifndef _BARYCENTRIC_COORDINATES_
#define _BARYCENTRIC_COORDINATES_

#include "Vec2.hpp"

/*
\brief Represents a point in Barycentric Coordinates of a triangle
*/
struct BarycentricCoordinates
{
  BarycentricCoordinates() = default;

  /*
  \brief Constructs a BarycentricCoordinates object from cartesian coordinates and the coordinates of the triangle
  \param cartesianCoordinates Cartesian coordinates of the point to reprent using barycentric coordinates
  \param trianglePoint1 Cartesian coordinates of the first point of the triangle
  \param trianglePoint2 Cartesian coordinates of the second point of the triangle
  \param trianglePoint3 Cartesian coordinates of the third point of the triangle
  \returns BarcentricCoordinates object
  */
  BarycentricCoordinates(const Vec2<float>& cartesianCoordinates, const Vec2<float>& trianglePoint1,
                         const Vec2<float>& trianglePoint2, const Vec2<float>& trianglePoint3);

  Vec2<float> p, q, r; //Cartesian coordinates of the points defining the triangle
  float alpha, beta, gamma; //Barycentric coordinates of the point withing the triangle
};

#endif