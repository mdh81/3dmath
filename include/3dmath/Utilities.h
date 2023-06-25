#pragma once
#include "Constants.h"
#include "Vector.h"
#include <cmath>
#include <random>

namespace math3d {

    class Utilities {
    public:
        [[nodiscard]]
        static float asDegrees(float radians) {
            return radians * Constants::radiansToDegrees;
        };

        template<typename T>
        [[nodiscard]]
        static bool isZero(T const val) {
            static_assert(std::is_floating_point<T>(),
                          "Only floating point types can be compared to zero using this function");
            return std::fabs(val) < Constants::tolerance;
        }

        template<typename T1, typename T2>
        [[nodiscard]]
        static bool areEqual(T1 const val1, T2 const val2) {
            static_assert(std::is_same<T1, T2>(), "Cannot compare dissimilar types");
            static_assert(std::is_floating_point<T1>(),
                          "Only floating point types can be compared using this function");
            return isZero(std::fabs(val2-val1));
        }

        template<unsigned size>
        [[nodiscard]]
        static Vector<float, size> getPerpendicular(Vector<float, size>& vector) {
            static_assert(size == 2 || size == 3, "Cannot compute perpendicular for higher dimensional vectors");
            if (size == 2) {
                return {-vector.y, vector.x};
            } else {
                if (areEqual(vector.dot(Constants::zAxis), 1.f)) {
                    return Constants::yAxis;
                } else {
                    return {-vector.y, vector.x, 0};
                }
            }
        }

        class RandomNumber {
        public:
            operator float() const {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float> dist(-100,+100);
                return dist(gen);
            }
        };

        class RandomVector {
        public:
            operator Vector3D<float>() {
                return {RandomNumber(), RandomNumber(), RandomNumber()};
            }
            operator Vector2D<float>() {
                return {RandomNumber(), RandomNumber()};
            }
        };

        using RandomPoint = RandomVector;
    };
}