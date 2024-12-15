#pragma once

#include <ostream>

namespace world::plane {

template <class T = int>
struct Vec2 {
    T x = 0;
    T y = 0;
};

using Vec2i = Vec2<int>;
using Vec2ll = Vec2<long long>;

template <class T>
inline Vec2<T> operator+(Vec2<T> lhs, Vec2<T> rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <class T>
inline Vec2<T> operator-(Vec2<T> lhs, Vec2<T> rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <class T>
inline Vec2<T> operator*(Vec2<T> lhs, int rhs) {
    return {lhs.x * rhs, lhs.y * rhs};
}

template <class T>
inline Vec2<T> operator*(int lhs, Vec2<T> rhs) {
    return {lhs * rhs.x, lhs * rhs.y};
}

template <class T>
inline Vec2<T>& operator+=(Vec2<T>& rhs, Vec2<T> lhs) {
    rhs.x += lhs.x;
    rhs.y += lhs.y;
    return rhs;
}

template <class T>
inline bool operator==(Vec2<T> lhs, Vec2<T> rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <class T>
inline bool operator!=(Vec2<T> lhs, Vec2<T> rhs) {
    return !(lhs == rhs);
}

template <class T>
inline std::ostream& operator<<(std::ostream& out, Vec2<T> v) {
    return out << "Vec2{x=" << v.x << ", y=" << v.y << "}";
}

}

template<class T>
struct std::hash<world::plane::Vec2<T>>
{
    std::size_t operator()(const world::plane::Vec2<T>& v) const noexcept
    {
        return hash<int>{}(v.x) ^ hash<int>{}(v.y);
    }
};