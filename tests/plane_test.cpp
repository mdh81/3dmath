#include "gtest/gtest.h"
#include "3dmath/primitives/Plane.h"
#include "TestSupport.h"

TEST(Plane, Construction) {
}

TEST(Plane, Getters) {
    math3d::Primitive::Point origin  = math3d::Utilities::RandomPoint();
    math3d::Primitive::Vector normal = math3d::Utilities::RandomVector();
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
    math3d::Plane {math3d::Constants::origin,
                   math3d::Constants::xAxis}.writeToSTL("Yz.stl");
    math3d::Plane {math3d::Constants::origin,
                   math3d::Constants::yAxis}.writeToSTL("Xz.stl");
    math3d::Plane {math3d::Constants::origin,
                   math3d::Constants::zAxis}.writeToSTL("Xy.stl");
    for (auto& fileName : { "Xy.stl", "Xz.stl", "Yz.stl"}) {
        ASSERT_TRUE(
                math3d::test::TestSupport::areBinarySTLFilesEqual(
                        fileName,
                        std::filesystem::path("baseline") / fileName)) << "Geometry in STL file is different";
    }
}
