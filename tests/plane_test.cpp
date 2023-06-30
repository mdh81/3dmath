#include "gtest/gtest.h"
#include "3dmath/primitives/Plane.h"
#include "TestSupport.h"

constexpr unsigned robustnessTestSampleCount = 100;

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
    math3d::Plane {math3d::constants::origin,
                   math3d::constants::xAxis}.writeToSTL("Yz.stl");
    math3d::Plane {math3d::constants::origin,
                   math3d::constants::yAxis}.writeToSTL("Xz.stl");
    math3d::Plane {math3d::constants::origin,
                   math3d::constants::zAxis}.writeToSTL("Xy.stl");
    for (auto& fileName : { "Xy.stl", "Xz.stl", "Yz.stl"}) {
        ASSERT_TRUE(
                math3d::test::TestSupport::areBinarySTLFilesEqual(
                        fileName,
                        std::filesystem::path("baseline") / fileName)) << "Geometry in STL file is different";
    }
}

TEST(Plane, PointProjection) {
    math3d::Plane plane {math3d::constants::origin, math3d::constants::zAxis};
    auto projectedPoint = plane.getProjection({0, 0, 5});
    ASSERT_FLOAT_EQ(projectedPoint.x, 0.f);
    ASSERT_FLOAT_EQ(projectedPoint.y, 0.f);
    ASSERT_FLOAT_EQ(projectedPoint.z, 0.f);
}

TEST(Plane, PointProjectionRobustness) {
    for (auto sample = 1u; sample <= robustnessTestSampleCount; ++sample) {
        math3d::ConvexPrimitive::Point pointInSpace = math3d::Utilities::RandomPoint();
        math3d::ConvexPrimitive::Point planeOrigin = math3d::Utilities::RandomPoint();
        math3d::ConvexPrimitive::Vector planeNormal = math3d::Utilities::RandomVector();
        auto plane = math3d::Plane{planeOrigin, planeNormal};
        auto projectedPoint = plane.getProjection(pointInSpace);
        auto planeVector = projectedPoint - planeOrigin;
        if (!math3d::Utilities::isZero(planeVector.dot(planeNormal))) {
            std::cerr << "Plane projection failed for plane " << plane << " and point " << pointInSpace << std::endl;
            std::cerr << "Projected point " << projectedPoint << std::endl;
            plane.writeToSTL("ProjectionFailure.stl");
        }
    }
}