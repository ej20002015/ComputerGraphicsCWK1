#ifndef _PARAMETERISED_LINE_
#define _PARAMETERISED_LINE_

struct vec2
{
  float x, y;

  friend vec2 operator+(const vec2& v1, const vec2& v2);
  friend vec2 operator+(const vec2& v1, float v2);
  friend vec2 operator+(float v1, const vec2& v2);
  friend vec2 operator-(const vec2& v1, const vec2& v2);
  friend vec2 operator*(const vec2& v1, const vec2& v2);
  friend vec2 operator*(const vec2& v1, float v2);
  friend vec2 operator*(float v1, const vec2& v2);

  vec2 perp();
  float dot(const vec2& v);
  
};

struct ParameterisedLine
{
  vec2 p, q;
};

#endif