#pragma once

#include "direction.hpp"

#include <cstddef>

namespace world::plane {

template <class T = int>
class Object {
public:
    explicit Object(Vec2<T> position) noexcept : pos_(position) {}
    Vec2<T> position() const noexcept { return pos_; }

    void move(Direction direction, int steps = 1) noexcept {
        pos_ += toVec2<T>(direction) * steps;
    }
    void set_position(Vec2<T> position) noexcept { pos_ = position; }
public:
    Vec2<T> pos_;
};

template <class T = int>
class OrientedObject : public Object<T> {
public:
    OrientedObject(Vec2<T> position, Direction direction) noexcept : Object<T>(position), dir_(direction) {}

    void do_steps(int steps = 1) noexcept {
        Object<T>::move(dir_, steps);
    }

    void rotate90CW() noexcept {
        // [0, 1; -1, 0] * [x, y]
        auto v = toVec2<T>(dir_);
        dir_ = fromVec2<T>({v.y, -v.x});
    }

    Direction direction() const noexcept { return dir_; }
    void set_direction(Direction dir) { dir_ = dir; }

private:

    Direction dir_;
};

template <class T>
std::ostream& operator<<(std::ostream& out, const OrientedObject<T>& obj) {
    return out << "OrientedObject{pos=" << obj.position() << ", dir=" << obj.direction() << "}";
}

}