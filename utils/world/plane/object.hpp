#pragma once

#include "direction.hpp"

#include <cstddef>

namespace world {
namespace plane {

class Object {
public:
    explicit Object(Vec2 position) noexcept : pos_(position) {}
    Vec2 position() const noexcept { return pos_; }

    void move(Direction direction, int steps = 1) noexcept {
        pos_ += toVec2(direction) * steps;
    }
    void set_position(Vec2 position) noexcept { pos_ = position; }
public:
    Vec2 pos_;
};

class OrientedObject : public Object {
public:
    OrientedObject(Vec2 position, Direction direction) noexcept : Object(position), dir_(direction) {}

    void do_steps(int steps = 1) noexcept {
        Object::move(dir_, steps);
    }

    void rotate90CW() noexcept {
        // [0, 1; -1, 0] * [x, y]
        auto v = toVec2(dir_);
        dir_ = fromVec2({v.y, -v.x});
    }

    Direction direction() const noexcept { return dir_; }
    void set_direction(Direction dir) { dir_ = dir; }

private:

    Direction dir_;
};

std::ostream& operator<<(std::ostream&, const OrientedObject&);

}
}