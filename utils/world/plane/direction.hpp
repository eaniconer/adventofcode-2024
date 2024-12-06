#pragma once

#include "vec2.hpp"
#include <array>

namespace world::plane {

enum class Direction {
    N = 0, // North
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW,
    No
};

inline Vec2 toVec2(Direction dir) {
    switch (dir)
    {
    case Direction::N: return {0, 1};
    case Direction::NE: return {1, 1};
    case Direction::E: return {1, 0};
    case Direction::SE: return {1, -1};
    case Direction::S: return {0, -1};
    case Direction::SW: return {-1, -1};
    case Direction::W: return {-1, 0};
    case Direction::NW: return {-1, 1};
    case Direction::No:
    default:
        return {0, 0};
    }
}

inline Direction fromVec2(Vec2 dir) {
    std::array<Direction, 3> dirs;
    if (dir.x < 0) {
        dirs = { Direction::SW, Direction::NW, Direction::W };
    } else if (dir.x > 0) {
        dirs = { Direction::SE, Direction::NE, Direction::E };
    } else {
        dirs = { Direction::S, Direction::N, Direction::No };
    }
    if (dir.y < 0) return dirs[0];
    if (dir.y > 0) return dirs[1];
    return dirs[2];
}

inline std::ostream& operator<<(std::ostream& out, Direction dir) {
#define PUT_DIRECTION_CASE(dir) case dir: return out << #dir
    switch (dir)
    {
    PUT_DIRECTION_CASE(Direction::N);  
    PUT_DIRECTION_CASE(Direction::NE);  
    PUT_DIRECTION_CASE(Direction::E);  
    PUT_DIRECTION_CASE(Direction::SE);
    PUT_DIRECTION_CASE(Direction::S); 
    PUT_DIRECTION_CASE(Direction::SW);  
    PUT_DIRECTION_CASE(Direction::W);  
    PUT_DIRECTION_CASE(Direction::NW);  
    PUT_DIRECTION_CASE(Direction::No);    
    default:
        return out;     
    }
#undef PUT_DIRECTION_CASE
}

}

template<>
struct std::hash<world::plane::Direction>
{
    std::size_t operator()(const world::plane::Direction& d) const noexcept
    {
        using T = std::underlying_type<world::plane::Direction>::type;
        return std::hash<T>{}(static_cast<T>(d));
    }
};