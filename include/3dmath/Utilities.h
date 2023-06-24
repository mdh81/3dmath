#pragma once
#include "Constants.h"
#include <cmath>

namespace math3d {
    inline float asDegrees(float radians) {
        return radians * radiansToDegrees;
    };

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
}