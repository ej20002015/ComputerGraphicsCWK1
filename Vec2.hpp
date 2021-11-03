#ifndef _VEC2_
#define _VEC2_

template<typename T>
struct Vec2
{
  T x, y;

  friend Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2);
  friend Vec2<T> operator+(const Vec2<T>& v1, float v2);
  friend Vec2<T> operator+(float v1, const Vec2<T>& v2);
  friend Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2);
  friend Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2);
  friend Vec2<T> operator*(const Vec2<T>& v1, float v2);
  friend Vec2<T> operator*(float v1, const Vec2<T>& v2);

  Vec2<T> perp();
  T dot(const Vec2<T>& v);
  
};

#endif