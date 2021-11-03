#ifndef _MAT2_2_
#define _MAT2_2_

#include "Vec2.hpp"

/*
\brief Represents a 2x2 matrix
*/
struct Mat2x2
{
  float elements[2][2]; //Elements of the matrix

  friend Vec2<float> operator*(const Mat2x2& mat, const Vec2<float>& vec);
  friend Mat2x2 operator*(float v, const Mat2x2& mat);

  /*
  \brief Calculates the inverse of the matrix
  \return If its invertible: the inverse of the matrix; If its not invertible: the original matrix
  */
  Mat2x2 inverse() const;

  /*
  \brief Calculates the determinant of the matrix
  \return The determinant of the matrix
  */
  float determinant() const;
};

#endif