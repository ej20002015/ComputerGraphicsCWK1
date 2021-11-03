#include "Vec2.hpp"

template<typename T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2)
{
  return { v1.x + v2.x, v1.y + v2.y };
}

template<typename T>
Vec2<T> operator+(const Vec2<T>& v1, float v2)
{
  return { v1.x + v2, v1.y + v2 };
}

template<typename T>
Vec2<T> operator+(float v1, const Vec2<T>& v2)
{
  return { v2.x + v1, v2.y + v1 };
}

template<typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2)
{
  return { v1.x - v2.x, v1.y - v2.y };
}

template<typename T>
Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2)
{
  return { v1.x * v2.x, v1.y * v2.y };
}

template<typename T>
Vec2<T> operator*(const Vec2<T>& v1, float v2)
{
  return { v1.x * v2, v1.y * v2 };
}

template<typename T>
Vec2<T> operator*(float v1, const Vec2<T>& v2)
{
  return { v2.x * v1, v2.y * v1 };
}

template<typename T>
Vec2<T> Vec2<T>::perp()
{
  return { -y, x };
}

template<typename T>
T Vec2<T>::dot(const Vec2& v)
{
  return (x * v.x) + (y * v.y);
}