#include "ParameterisedLine.hpp"

vec2 operator+(const vec2& v1, const vec2& v2)
{
  return { v1.x + v2.x, v1.y + v2.y };
}

vec2 operator+(const vec2& v1, float v2)
{
  return { v1.x + v2, v1.y + v2 };
}

vec2 operator+(float v1, const vec2& v2)
{
  return { v2.x + v1, v2.y + v1 };
}

vec2 operator-(const vec2& v1, const vec2& v2)
{
  return { v1.x - v2.x, v1.y - v2.y };
}

vec2 operator*(const vec2& v1, const vec2& v2)
{
  return { v1.x * v2.x, v1.y * v2.y };
}

vec2 operator*(const vec2& v1, float v2)
{
  return { v1.x * v2, v1.y * v2 };
}

vec2 operator*(float v1, const vec2& v2)
{
  return { v2.x * v1, v2.y * v1 };
}

vec2 vec2::perp()
{
  return { -y, x };
}

float vec2::dot(const vec2& v)
{
  return (x * v.x) + (y * v.y);
}