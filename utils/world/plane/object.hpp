#pragma once

#include "direction.hpp"

#include <cstddef>

namespace world::plane {

template <class T = int>
class Object {
public:
    explicit Object(Vec2<T> position) noexcept : pos_(position) {}
    Vec2<T> position() const noexcept { return pos_; }
    T width() const noexcept { return size_.x; }
    T height() const noexcept { return size_.y; }

    void move(Direction direction, int steps = 1) noexcept {
        pos_ = plane::shift(pos_, direction, steps);
    }
    void set_position(Vec2<T> position) noexcept { pos_ = position; }
    void set_size(Vec2<T> sz) noexcept { size_ = sz; }
public:
    Vec2<T> pos_;
    Vec2<T> size_ = {1, 1};
};

template <class T = int>
class OrientedObject : public Object<T> {
public:
    using position_type = Vec2<T>;
    using direction_type = Direction;

    OrientedObject(Vec2<T> position, Direction direction) noexcept : Object<T>(position), dir_(direction) {}

    void do_steps(int steps = 1) noexcept {
        Object<T>::move(dir_, steps);
    }

    void rotate90CW() noexcept {
        // [0, 1; -1, 0] * [x, y]
        auto v = toVec2<T>(dir_);
        dir_ = fromVec2<T>({v.y, -v.x});
    }

    void rotate90CCW() noexcept {
        // [0, -1; 1, 0] * [x, y]
        auto v = toVec2<T>(dir_);
        dir_ = fromVec2<T>({-v.y, v.x});
    }

    Direction direction() const noexcept { return dir_; }
    void set_direction(Direction dir) { dir_ = dir; }

private:

    Direction dir_;
};

template <class T>
bool has_collision_on_move(OrientedObject<T> movable, const Object<T>& obj) {
    movable.do_steps(1);

    auto mp = movable.position();
    auto op = obj.position();
    for (T mi = 0; mi < movable.height(); ++mi) { // todo: implement more effective algorithm
        for (T mj = 0; mj < movable.width(); ++mj) {
            for (T oi = 0; oi < obj.height(); ++oi) {
                for (T oj = 0; oj < obj.width(); ++oj) {
                    if ((mp + Vec2<T>{mj, mi}) == (op + Vec2<T>{oj, oi})) {
                        return true;
                    }
                }
            }
        }
    }
    return false;

}

template <class T>
std::ostream& operator<<(std::ostream& out, const OrientedObject<T>& obj) {
    return out << "OrientedObject{pos=" << obj.position() << ", dir=" << obj.direction() << "}";
}

}