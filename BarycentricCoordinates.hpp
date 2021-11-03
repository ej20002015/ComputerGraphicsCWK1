#ifndef _BARYCENTRIC_COORDINATES_
#define _BARYCENTRIC_COORDINATES_

#include "Vec2.hpp"

//Represents a point with barycentric coordinates of a triangle
struct BarycentricCoordinates
{
  BarycentricCoordinates() = default;
  BarycentricCoordinates(const Vec2& cartesianCoordinates, const Vec2& trianglePoint1,
                         const Vec2& trianglePoint2, const Vec2& trianglePoint3);

  Vec2 p, q, r;
  float alpha, beta, gamma;
};

#endif