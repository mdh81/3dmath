#include "gtest/gtest.h"
#include "3dmath/Vector.h"
#include "3dmath/OtherTypes.h"
#include "3dmath/primitives/Sphere.h"
#include <vector>
using namespace std;
using namespace math3d;

TEST(Sphere, Getters) {
    Sphere sphere({10.f, 10.f, 10.f}, 10.f);
    ASSERT_FLOAT_EQ(sphere.getRadius(), 10) << "Sphere radius is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().x, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().y, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().z, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getResolution(), 16) << "Sphere resolution is wrong";
    ASSERT_FLOAT_EQ(sphere.getSubdivisions(), 16) << "Sphere subdivisions is wrong";
}

TEST(Sphere, GeometryGeneration) {
    Sphere sphere({10.f, 10.f, 10.f}, 10.f);
    sphere.generateGeometry();
    auto resolution = sphere.getResolution();
    auto subdivisions = sphere.getSubdivisions();
    ASSERT_EQ(sphere.getVertices().size(), resolution * (subdivisions - 1) + 2);
}

