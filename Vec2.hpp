#ifndef _VEC2_
#define _VEC2_

/*
\brief Represents a two dimensional vector
\tparam T The typename of the underlying data type to store in the vector (should be int or float)
*/
template<typename T>
struct Vec2
{
  T x, y; //The elements of the vector

  /*
  \brief Compute the perpindicular dot product of the vector
  \return The perpindicular dot product of the vector
  */
  Vec2<T> perp()
  {
    return { -y, x };
  }

  /*
  \brief Compute the dot product of this vector and another
  \param v The other vector to perform a dot product with
  \return The dot product of this and v
  */
  T dot(const Vec2<T>& v)
  {
    return (x * v.x) + (y * v.y);
  }
  
};


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

#endif