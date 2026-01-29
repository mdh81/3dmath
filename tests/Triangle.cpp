#include "3dmath/primitives/Triangle.h"

#include <algorithm>

#include "gtest/gtest.h"
#include "3dmath/Utilities.h"
#include <ranges>

using namespace math3d;

TEST(Triangle, CrossProductBasedArea) {
    Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
    auto [a, b, c] = tri.getPoints();
    auto ab = b - a;
    auto ac = c - a;
    auto theta = atan2((ab * ac).length(), ab.dot(ac));
    auto base = ab.length();
    auto height = ac.length() * sin(theta);
    ASSERT_NEAR(0.5 * base * height, tri.getArea(), constants::tolerance);
    ASSERT_NEAR(0.5 * (ab * ac).length(), tri.getArea(), constants::tolerance);
}

TEST(Triangle, PointContainment) {
    size_t constexpr numTestRuns {10};
    for (size_t i = 0; i < numTestRuns; ++i) {
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto [a, b, c] = tri.getPoints();
        ASSERT_TRUE(tri.isPointInTriangle(a));
        ASSERT_TRUE(tri.isPointInTriangle(b));
        ASSERT_TRUE(tri.isPointInTriangle(c));
    }
    for (size_t i = 0; i < numTestRuns; ++i) {
        double u = Utilities::RandomNumber{0.0, 1.0};
        double v = Utilities::RandomNumber{0.0, 1.0};
        double w = 1 - u - v;
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto pt = tri.getPoints()[0] * u + tri.getPoints()[1] * v + tri.getPoints()[2] * w;
        ASSERT_TRUE(tri.isPointInTriangle(pt));
    }
}

TEST(Triangle, BaryCentricCoordinates) {
    for (int i = 0; i < 10; ++i) {
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto [a, b, c] = tri.getPoints();
        auto barycentricCoordinates = tri.getBarycentricCoordinates(a);
        ASSERT_NEAR(0, barycentricCoordinates.x, constants::tolerance);
        barycentricCoordinates = tri.getBarycentricCoordinates(b);
        ASSERT_NEAR(0, barycentricCoordinates.y, constants::tolerance);
        barycentricCoordinates = tri.getBarycentricCoordinates(c);
        ASSERT_NEAR(0, barycentricCoordinates.z, constants::tolerance);
    }
}