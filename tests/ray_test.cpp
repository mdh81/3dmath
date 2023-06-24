#include "gtest/gtest.h"
#include "3dmath/primitives/Ray.h"
using namespace std;
using namespace math3d;

TEST(Ray, DistanceToPoint) {
    // Same direction
    Ray r({0,0,0}, {1,0,0});
    ASSERT_FLOAT_EQ(10, r.distanceToPoint({10, 10, 0}));

    // Different direction
    ASSERT_FLOAT_EQ(10, r.distanceToPoint({-10, -10, 0}));

    // TODO: Different plane
    //Vector3D<float> point = {-10, -10, 10};
    //ASSERT_FLOAT_EQ(point.length(), r.distanceToPoint(point));
}
