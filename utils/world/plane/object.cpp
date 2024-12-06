#include "object.hpp"

namespace world::plane {

std::ostream& operator<<(std::ostream& out, const OrientedObject& obj) {
    return out << "OrientedObject{pos=" << obj.position() << ", dir=" << obj.direction() << "}";
}

}
