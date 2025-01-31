#include <format>

namespace njin::math {
    template<typename T>
    njVec3<T>::njVec3(T x, T y, T z) : x{ x }, y{ y }, z{ z } {}

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const njVec3<T>& vec) {
        os << std::format("({}, {}, {})", vec.x, vec.y, vec.z);
        return os;
    }
}  // namespace njin::math
