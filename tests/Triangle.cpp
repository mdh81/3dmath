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
        ASSERT_TRUE(
            tri.isPointInTriangle(a) &&
            tri.isPointInTriangle(b) &&
            tri.isPointInTriangle(c)) << "Triangle vertex incorrectly classified as outside the triangle";
    }
    for (size_t i = 0; i < numTestRuns; ++i) {
        std::array<double, 3> weights = {Utilities::PositiveRandomNumber{}, Utilities::PositiveRandomNumber{}, Utilities::PositiveRandomNumber{} };
        double sum{};
        std::ranges::for_each(weights, [&](double const weight) {
            sum += weight;
        });
        std::ranges::for_each(weights, [&](double& weight) {
            weight /= sum;
        });
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto pt = tri.getPoints()[0] * weights[0] + tri.getPoints()[1] * weights[1] + tri.getPoints()[2] * weights[2];
        ASSERT_TRUE(tri.isPointInTriangle(pt))
            << "Convex barycentric combination of triangle vertices cannot be outside the triangle";
    }
    for (size_t i = 0; i < numTestRuns; ++i) {
        double u = Utilities::NegativeRandomNumber{};
        double v = Utilities::PositiveRandomNumber{};
        double w = Utilities::NegativeRandomNumber{};
        Triangle tri { Utilities::RandomPoint{}, Utilities::RandomPoint{}, Utilities::RandomPoint{} };
        auto pt = tri.getPoints()[0] * u + tri.getPoints()[1] * v + tri.getPoints()[2] * w;
        ASSERT_FALSE(tri.isPointInTriangle(pt)) << "Only convex barycentric combinations of triangle vertices can be inside the triangle";
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