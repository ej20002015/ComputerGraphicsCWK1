#include "Mat2x2.hpp"

Vec2<float> operator*(const Mat2x2& mat, const Vec2<float>& vec)
{
  float xComponent = mat.elements[0][0] * vec.x + mat.elements[0][1] * vec.y;
  float yComponent = mat.elements[1][0] * vec.x + mat.elements[1][1] * vec.y;
  return { xComponent, yComponent };
}

Mat2x2 operator*(float v, const Mat2x2& mat)
{
  Mat2x2 matReturn;
  matReturn.elements[0][0] = mat.elements[0][0] * v;
  matReturn.elements[0][1] = mat.elements[0][1] * v;
  matReturn.elements[1][0] = mat.elements[1][0] * v;
  matReturn.elements[1][1] = mat.elements[1][1] * v;

  return matReturn;
}

Mat2x2 Mat2x2::inverse() const
{
  float deter = determinant();
  if (deter)
  {
    float a = elements[0][0];
    float b = elements[0][1];
    float c = elements[1][0];
    float d = elements[1][1];

    Mat2x2 inverse;
    inverse.elements[0][0] =  d;
    inverse.elements[0][1] = -b;
    inverse.elements[1][0] = -c;
    inverse.elements[1][1] =  a;

    return (1.0f / deter) * inverse;
  }
  else
    return *this;
}

float Mat2x2::determinant() const
{
  return elements[0][0] * elements[1][1] - elements[0][1] * elements[1][0];
}