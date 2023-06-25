#pragma once

#include <cmath>
#include <string>
#include <initializer_list>
#include "Vector.h"
#include "Utilities.h"

namespace math3d {
    template <typename T>
    struct Extent {
        T min;
        T max;
        T length() const { return max - min; }
        T center() const { return 0.5f*(max + min); }
    };

    template<typename T>
    struct Bounds3D {
        Extent<T> x;
        Extent<T> y;
        Extent<T> z;

        Bounds3D() {
            reset();
        }

        Bounds3D(std::initializer_list<std::initializer_list<T>> const& initializerList) {
            std::string invalidListErrorMessage =
                    "Incorrect initializer list. "
                    "Use format:"
                    "{{Minimum X, Minimum Y, Minimum Z}, {Maximum X, Maximum Y, Maximum Z}} "
                    "For 2D bounds, Z extents can be skipped";
            bool formatIsCorrect =
                    initializerList.size() == 2 &&
                    (data(initializerList)[0].size() == 2 ||  data(initializerList)[0].size() == 3) &&
                    data(initializerList)[0].size() == data(initializerList)[1].size();
            if (!formatIsCorrect) {
                throw std::runtime_error(invalidListErrorMessage);
            }

            x.min = data(data(initializerList)[0])[0];
            y.min = data(data(initializerList)[0])[1];
            z.min = data(initializerList)[0].size() == 2 ? static_cast<T>(0) : data(data(initializerList)[0])[2];

            x.max = data(data(initializerList)[1])[0];
            y.max = data(data(initializerList)[1])[1];
            z.max = data(initializerList)[0].size() == 2 ? static_cast<T>(0) : data(data(initializerList)[1])[2];
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

        [[nodiscard]] Vector3D<T> center() const {
            return {x.center(), y.center(), z.center()};
        }

        void reset() {
            x.min = std::numeric_limits<T>::max();
            y.min = std::numeric_limits<T>::max();
            z.min = std::numeric_limits<T>::max();
            x.max = -x.min;
            y.max = -y.min;
            z.max = -z.min;
        }

        bool contains(Vector<T, 3> const& point) const {
            bool withinXExtent = (point.x > x.min && point.x < x.max) ||
                                 fabs(point.x - x.min) < 1e-6 ||
                                 fabs(point.x - x.max) < 1e-6;

            bool withinYExtent = (point.y > y.min && point.y < y.max) ||
                                 fabs(point.y - y.min) < 1e-6 ||
                                 fabs(point.y - y.max) < 1e-6;

            bool withinZExtent = (point.z > z.min && point.z < z.max) ||
                                 fabs(point.z - z.min) < 1e-6 ||
                                 fabs(point.z - z.max) < 1e-6;

            return withinXExtent && withinYExtent && withinZExtent;
        }
    };

    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, Bounds3D<T> const& bounds3D) {
        os << "Min:[" << bounds3D.x.min << "," << bounds3D.y.min << "," << bounds3D.z.min << "] ";
        os << "Max:[" << bounds3D.y.max << "," << bounds3D.y.max << "," << bounds3D.z.max << "]";
        return os;
    }

    template<typename T>
    bool isZero(Extent<T> const extent) {
        return Utilities::isZero(std::fabs(extent.max - extent.min));
    }

    enum class IntersectionStatus {
        Intersects,
        NoIntersection,
        Skew
    };
    struct IntersectionResult {
        IntersectionStatus status;
        Vector3D<float> intersectionPoint;
    };

}
