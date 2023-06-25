#include "gtest/gtest.h"
#include "3dmath/primitives/Plane.h"
#include "TestSupport.h"

TEST(Plane, Construction) {
}

TEST(Plane, Getters) {
    math3d::Primitive::Point origin;
    origin.x = math3d::RandomNumber();
    origin.y = math3d::RandomNumber();
    origin.z = math3d::RandomNumber();
    math3d::Primitive::Vector normal;
    normal.x = math3d::RandomNumber();
    normal.y = math3d::RandomNumber();
    normal.z = math3d::RandomNumber();
    math3d::Plane p{origin, normal};
    ASSERT_FLOAT_EQ(p.getOrigin().x, origin.x);
    ASSERT_FLOAT_EQ(p.getOrigin().y, origin.y);
    ASSERT_FLOAT_EQ(p.getOrigin().z, origin.z);
    // Normal should be normalized by the constructor
    ASSERT_FLOAT_EQ(p.getNormal().x, normal.normalize().x);
    ASSERT_FLOAT_EQ(p.getNormal().y, normal.normalize().y);
    ASSERT_FLOAT_EQ(p.getNormal().z, normal.normalize().z);

}

TEST(Plane, GeometryGeneration) {
    math3d::Plane {{0, 0, 0}, {1, 0, 0}}.writeToSTL("Yz.stl");
    math3d::Plane {{0, 0, 0}, {0, 1, 0}}.writeToSTL("Xz.stl");
    math3d::Plane {{0, 0, 0}, {0, 0, 100}}.writeToSTL("Xy.stl");
    for (auto& fileName : { "Xy.stl", "Xz.stl", "Yz.stl"}) {
        ASSERT_TRUE(
                math3d::test::TestSupport::areBinarySTLFilesEqual(
                        fileName,
                        std::filesystem::path("baseline") / fileName)) << "Geometry in STL file is different";
    }
}