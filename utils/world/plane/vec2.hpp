#pragma once

#include <ostream>

namespace world::plane {

struct Vec2 {
    int x = 0;
    int y = 0;
};

inline Vec2 operator+(Vec2 lhs, Vec2 rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vec2 operator-(Vec2 lhs, Vec2 rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Vec2 operator*(Vec2 lhs, int rhs) {
    return {lhs.x * rhs, lhs.y * rhs};
}

inline Vec2 operator*(int lhs, Vec2 rhs) {
    return {lhs * rhs.x, lhs * rhs.y};
}

inline Vec2& operator+=(Vec2& rhs, Vec2 lhs) {
    rhs.x += lhs.x;
    rhs.y += lhs.y;
    return rhs;
}

inline bool operator==(Vec2 lhs, Vec2 rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(Vec2 lhs, Vec2 rhs) {
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& out, Vec2 v) {
    return out << "Vec2{x=" << v.x << ", y=" << v.y << "}";
}

}

template<>
struct std::hash<world::plane::Vec2>
{
    std::size_t operator()(const world::plane::Vec2& v) const noexcept
    {
        return hash<int>{}(v.x) ^ hash<int>{}(v.y);
    }
};