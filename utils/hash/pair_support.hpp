
#include <utility>

template<class A, class B>
struct std::hash<std::pair<A, B>>
{
    size_t operator()(const std::pair<A, B>& p) const {
        return std::hash<A>{}(p.first) ^ std::hash<B>{}(p.second);
    }
};
