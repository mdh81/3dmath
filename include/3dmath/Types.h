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

        Bounds3D(std::initializer_list<T> const& initializerList) {
            std::string invalidListErrorMessage =
                    "Incorrect initializer list. "
                    "Use format {{xmin, ymin, zmin}, {xmax, ymax, zmax}}";
            if (initializerList.size() != 2) {
                throw std::runtime_error(invalidListErrorMessage);
            }
            for (auto i : {0, 1}) {
                if (data(initializerList)[i].size() != 3) {
                    throw std::runtime_error(invalidListErrorMessage);
                }
            }
            x.min = data(data(initializerList)[0])[0];
            y.min = data(data(initializerList)[0])[1];
            z.min = data(data(initializerList)[0])[2];

            x.max = data(data(initializerList)[1])[0];
            y.max = data(data(initializerList)[1])[1];
            z.max = data(data(initializerList)[1])[2];
        }

        Bounds3D(std::initializer_list<std::initializer_list<T>> const& initializerList) {
            std::string invalidListErrorMessage =
                "Incorrect initializer list. "
                "Use format {{xmin, xmax}, {ymin, ymax}, {zmin, zmax}}";
            if (initializerList.size() != 3) {
                throw std::runtime_error(invalidListErrorMessage);
            }
            for (auto i : {0, 1, 2}) {
                if (data(initializerList)[i].size() != 2) {
                    throw std::runtime_error(invalidListErrorMessage);
                }
            }
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

        T length() const {
            return sqrt((x.length() * x.length()) + (y.length() * y.length()) + (z.length() * z.length()));
        }

        friend std::ostream& operator<<(std::ostream& os, Bounds3D<T> const& bounds3D);
    };

    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, Bounds3D<T> const& bounds3D) {
        os << "Min:[" << bounds3D.x.min << "," << bounds3D.y.min << "," << bounds3D.z.min << "] ";
        os << "Max:[" << bounds3D.y.max << "," << bounds3D.y.max << "," << bounds3D.z.max << "]";
        return os;
    }

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
