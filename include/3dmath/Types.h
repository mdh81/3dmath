#pragma once

#include <cmath>

namespace math3d {
    template <typename T>
    struct Extent {
        T min;
        T max;
        T length() const { return max - min; }
    };

    template<typename T>
    struct Bounds3D {
        Extent<T> x;
        Extent<T> y;
        Extent<T> z;
    };

    constexpr float tolerance = 1e-6;

    template<typename T>
    bool isZero(T const val) {
        static_assert(std::is_floating_point<T>(),
                      "Only floating point types can be compared to zero using this function");
        return std::fabs(val) < tolerance;
    }

    template<typename T1, typename T2>
    bool areEqual(T1 const val1, T2 const val2) {
        static_assert(std::is_same<T1, T2>(), "Cannot compare dissimilar types");
        static_assert(std::is_floating_point<T1>(),
                      "Only floating point types can be compared using this function");
        return isZero(std::fabs(val2-val1));
    }

    template<typename T>
    bool isZero(Extent<T> const extent) {
        return isZero(std::fabs(extent.max - extent.min));
    }

}
