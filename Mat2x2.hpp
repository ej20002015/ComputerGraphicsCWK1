#ifndef _MAT2_2_
#define _MAT2_2_

#include "Vec2.hpp"

struct Mat2x2
{
  float elements[2][2];

  friend Vec2<float> operator*(const Mat2x2& mat, const Vec2<float>& vec);
  friend Mat2x2 operator*(float v, const Mat2x2& mat);

  Mat2x2 inverse() const;
  float determinant() const;
};

#endif