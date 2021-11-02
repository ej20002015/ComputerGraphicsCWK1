#include "Vec2.hpp"

Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
  return { v1.x + v2.x, v1.y + v2.y };
}

Vec2 operator+(const Vec2& v1, float v2)
{
  return { v1.x + v2, v1.y + v2 };
}

Vec2 operator+(float v1, const Vec2& v2)
{
  return { v2.x + v1, v2.y + v1 };
}

Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
  return { v1.x - v2.x, v1.y - v2.y };
}

Vec2 operator*(const Vec2& v1, const Vec2& v2)
{
  return { v1.x * v2.x, v1.y * v2.y };
}

Vec2 operator*(const Vec2& v1, float v2)
{
  return { v1.x * v2, v1.y * v2 };
}

Vec2 operator*(float v1, const Vec2& v2)
{
  return { v2.x * v1, v2.y * v1 };
}

Vec2 Vec2::perp()
{
  return { -y, x };
}

float Vec2::dot(const Vec2& v)
{
  return (x * v.x) + (y * v.y);
}