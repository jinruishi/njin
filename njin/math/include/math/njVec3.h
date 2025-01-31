#pragma once
#include <iostream>

namespace njin::math {
    template<typename T>
    class njVec3 {
        public:
        njVec3(T x, T y, T z);

        njVec3() = default;

        T x;
        T y;
        T z;

        friend std::ostream& operator<< <>(std::ostream& os,
                                           const njVec3<T>& vec);
    };

    using njVec3f = njVec3<float>;
}  // namespace njin::math

#include "math/njVec3.tpp"
