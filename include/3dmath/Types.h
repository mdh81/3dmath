#pragma once

#include <cmath>

namespace math3d {
    template <typename T>
    struct Extent {
        T min;
        T max;
        T length() const { return max - min; }
        T center() const { 0.5f*(max + min); }
    };

    template<typename T>
    struct Bounds3D {
        Extent<T> x;
        Extent<T> y;
        Extent<T> z;

        Bounds3D(std::initializer_list<std::initializer_list<T>> const& initializerList){
            x.min = data(data(initializerList)[0])[0];
            x.max = data(data(initializerList)[0])[1];

            y.min = data(data(initializerList)[1])[0];
            y.max = data(data(initializerList)[1])[1];

            z.min = data(data(initializerList)[2])[0];
            z.max = data(data(initializerList)[2])[1];
        }

        Bounds3D() {
            x.min = std::numeric_limits<float>::max();
            y.min = std::numeric_limits<float>::max();
            z.min = std::numeric_limits<float>::max();
            x.max = -x.min;
            y.max = -y.min;
            z.max = -z.min;
        }

        // Builds a symmetric bounding box where each side is
        // of the specified length
        explicit Bounds3D(T const sideLength) {
            x.min = -0.5f * sideLength;
            x.max = +0.5f * sideLength;
            y.min = -0.5f * sideLength;
            y.max = +0.5f * sideLength;
            z.min = -0.5f * sideLength;
            z.max = +0.5f * sideLength;
        }

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
