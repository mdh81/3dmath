#include "3dmath/primitives/Triangle.h"

#include <algorithm>

#include "gtest/gtest.h"
#include "3dmath/Utilities.h"
#include <ranges>

using namespace math3d;

TEST(Triangle, CrossProductBasedArea) {
    Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
    auto [a, b, c] = tri.getPoints();
    auto ab = b -a;
    auto ac = c -a;
    auto theta = atan2((ab * ac).length(), ab.dot(ac));
    auto base = (b-a).length();
    auto height = (c-a).length() * sin(theta);
    ASSERT_NEAR(0.5 * base * height, tri.getArea(), constants::Tolerance);
    ASSERT_NEAR(0.5 * (ab * ac).length(), tri.getArea(), constants::Tolerance);
}

TEST(Triangle, PointContainment) {
    for (int i = 0; i < 10; ++i) {
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto [a, b, c] = tri.getPoints();
        ASSERT_TRUE(tri.isPointInTriangle(a));
        ASSERT_TRUE(tri.isPointInTriangle(b));
        ASSERT_TRUE(tri.isPointInTriangle(c));
        ASSERT_FALSE(tri.isPointInTriangle(b - a + Utilities::getPerpendicular(b - a)));
        ASSERT_FALSE(tri.isPointInTriangle(c - b + Utilities::getPerpendicular(c - b)));
        ASSERT_FALSE(tri.isPointInTriangle(c - a  + Utilities::getPerpendicular(c - a)));
    }
}

TEST(Triangle, BaryCentricCoordinates) {
    for (int i = 0; i < 10; ++i) {
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto [a, b, c] = tri.getPoints();
        auto barycentricCoordinates = tri.getBarycentricCoordinates(a);
        ASSERT_TRUE(std::ranges::any_of(barycentricCoordinates.getComponents(), [](auto component) {
            return fabs(component) < math3d::constants::Tolerance;
        }));
    }
}