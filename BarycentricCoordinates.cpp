#include "BarycentricCoordinates.hpp"

#include "Mat2x2.hpp"

BarycentricCoordinates::BarycentricCoordinates(const Vec2<float>& cartesianCoordinates, const Vec2<float>& trianglePoint1,
                                               const Vec2<float>& trianglePoint2, const Vec2<float>& trianglePoint3)
                                               : p(trianglePoint1), q(trianglePoint2), r(trianglePoint3)
{
  //Construct the t matrix
  Mat2x2 T;

  T.elements[0][0] = p.x - r.x;
  T.elements[0][1] = q.x - r.x;
  T.elements[1][0] = p.y - r.y;
  T.elements[1][1] = q.y - r.y;

  //Construct the b vector on the right hand side of the system of equations
  Vec2<float> b =
  {
    cartesianCoordinates.x - r.x,
    cartesianCoordinates.y - r.y,
  };

  //Compute alpha, beta and gamma by multiplying both sides by T inverse
  Vec2<float> alphaAndBeta = T.inverse() * b;

  alpha = alphaAndBeta.x;
  beta = alphaAndBeta.y;
  gamma = 1 - (alpha + beta);
}