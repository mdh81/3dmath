#include "gtest/gtest.h"
#include "3dmath/primitives/Ray.h"
#include "3dmath/Constants.h"

TEST(Ray, PerpendicularDistanceToPoint) {
    // Same direction
    math3d::Ray r({0,0,0}, {1,0,0});
    ASSERT_FLOAT_EQ(10, r.distanceToPoint({10, 10, 0}));

    // Different direction
    ASSERT_FLOAT_EQ(10, r.distanceToPoint({-10, -10, 0}));

    // Special cases:
}

TEST(Ray, DistanceToPointOnDifferentPlane) {

}

TEST(Ray, IntersectionWithRay) {
    math3d::Ray r1(math3d::constants::origin, math3d::constants::xAxis);
}
