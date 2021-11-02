#ifndef _VEC2_
#define _VEC2_

struct Vec2
{
  float x, y;

  friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
  friend Vec2 operator+(const Vec2& v1, float v2);
  friend Vec2 operator+(float v1, const Vec2& v2);
  friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
  friend Vec2 operator*(const Vec2& v1, const Vec2& v2);
  friend Vec2 operator*(const Vec2& v1, float v2);
  friend Vec2 operator*(float v1, const Vec2& v2);

  Vec2 perp();
  float dot(const Vec2& v);
  
};

#endif