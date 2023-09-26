#pragma once
#include "Constants.h"
#include "Vector.h"
#include <cmath>
#include <random>
#include <chrono>
#include <iostream>

namespace math3d {

    class Utilities {
    public:
        template<typename T>
        [[nodiscard]]
        static T asDegrees(T radians) {
            return radians * constants::radiansToDegrees;
        };

        template<typename T>
        [[nodiscard]]
        static T asRadians(T degrees) {
            return degrees / constants::radiansToDegrees;
        };

        template<typename T>
        [[nodiscard]]
        static bool isZero(T const val) {
            static_assert(std::is_floating_point<T>(),
                          "Only floating point types can be compared to zero using this function");
            return std::fabs(val) < constants::tolerance;
        }

        template<typename T1, typename T2>
        [[nodiscard]]
        static bool areEqual(T1 const val1, T2 const val2) {
            static_assert(std::is_same<T1, T2>(), "Cannot compare dissimilar types");
            static_assert(std::is_floating_point<T1>(),
                          "Only floating point types can be compared using this function");
            return isZero(std::fabs(val2-val1));
        }

        template<typename T1, typename T2>
        [[nodiscard]]
        static bool isLessThanOrEqual(T1 const val1, T2 const val2) {
            return val1 < val2 || areEqual(val1, val2);
        }

        template<typename T, unsigned size>
        [[nodiscard]] static bool areParallel(Vector<T, size> const& v1, Vector<T, size> const& v2) {
            return areEqual(v1.dot(v2), 1.0);
        }

        template<typename T, unsigned size>
        [[nodiscard]] static T distanceBetween(Vector<T, size> const& point1, Vector<T, size> const& point2) {
            auto dx = point1.x - point2.x;
            auto dy = point1.y - point2.y;
            auto dz = point1.z - point2.z;
            return sqrt((dx*dx) + (dy*dy) + (dz*dz));
        }

        // TODO: Derive this
        template<typename T, unsigned size>
        [[nodiscard]]
        static Vector<T, size> getPerpendicular(Vector<T, size>& vector) {
            static_assert(size == 2 || size == 3, "Cannot compute perpendicular for higher dimensional vectors");
            if (size == 2) {
                return {-vector.y, vector.x};
            } else {
                if (areParallel(vector, Vector<double, size>(constants::zAxis))) {
                    return constants::yAxis;
                } else {
                    return {-vector.y, vector.x, 0};
                }
            }
        }

        static float asFloat(double value) {
            return std::round(value / constants::tolerance) * constants::tolerance;
        }

        static Vector3<float> asFloat(Vector3<double> const& doubleVector) {
            return {asFloat(doubleVector.x), asFloat(doubleVector.y), asFloat(doubleVector.z)};
        }

        static constexpr double defaultRandomNumberRangeMin = -100;
        static constexpr double defaultRandomNumberRangeMax = +100;

        class RandomNumber {
        public:
            RandomNumber(double rangeMin = defaultRandomNumberRangeMin, double rangeMax = defaultRandomNumberRangeMax)
            : rangeMin(rangeMin)
            , rangeMax(rangeMax) {};
            operator double() const { // NOLINT
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<double> dist(rangeMin, rangeMax);
                return dist(gen);
            }
            double rangeMin;
            double rangeMax;
        };

        class RandomVector {
        public:
            operator Vector3<double>() { // NOLINT
                return {RandomNumber(), RandomNumber(), RandomNumber()};
            }
            operator Vector2<double>() { // NOLINT
                return {RandomNumber(), RandomNumber()};
            }
        };

        using RandomPoint = RandomVector;

        class Timer {
        public:
            Timer(char const* op, std::ostream& os)
            : operation(op)
            , outputStream(os) {
                start = std::chrono::high_resolution_clock::now();
            }
            ~Timer() {
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                outputStream << operation << " in " <<  duration.count() << " milliseconds" << std::endl;
            }
            Timer(Timer const&) = delete;
            Timer& operator=(Timer const&) = delete;
            Timer(Timer&&) = delete;
            Timer& operator=(Timer&&) = delete;
        private:
            std::chrono::steady_clock::time_point start;
            std::string operation;
            std::ostream& outputStream;
        };

    };
}