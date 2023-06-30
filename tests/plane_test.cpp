#include "gtest/gtest.h"
#include "3dmath/primitives/Plane.h"
#include "TestSupport.h"

constexpr unsigned robustnessTestSampleCount = 100;

TEST(Plane, Getters) {
    math3d::types::Point3D origin  = math3d::Utilities::RandomPoint();
    math3d::types::Vector3D normal = math3d::Utilities::RandomVector();
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
                   math3d::constants::xAxis}.writeToFile("Yz.stl");
    math3d::Plane {math3d::constants::origin,
                   math3d::constants::yAxis}.writeToFile("Xz.stl");
    math3d::Plane {math3d::constants::origin,
                   math3d::constants::zAxis}.writeToFile("Xy.stl");
    for (auto& fileName : { "Xy.stl", "Xz.stl", "Yz.stl"}) {
        ASSERT_TRUE(
                math3d::test::TestSupport::areBinarySTLFilesEqual(
                        fileName,
                        std::filesystem::path("baseline") / fileName)) << "Geometry in STL file is different";
    }
}

TEST(Plane, PointProjection) {
    for (auto sample = 1u; sample <= robustnessTestSampleCount; ++sample) {
        math3d::types::Point3D pointInSpace = math3d::Utilities::RandomPoint();
        math3d::types::Point3D planeOrigin = math3d::Utilities::RandomPoint();
        math3d::types::Vector3D planeNormal = math3d::Utilities::RandomVector();
        auto plane = math3d::Plane{planeOrigin, planeNormal};
        auto projectedPoint = plane.getProjection(pointInSpace);
        auto planeVector = projectedPoint - planeOrigin;
        if (!math3d::Utilities::isZero(planeVector.dot(planeNormal))) {
            std::cerr << "Plane projection failed for plane " << plane << " and point " << pointInSpace << std::endl;
            std::cerr << "Projected point " << projectedPoint << std::endl;
            std::cerr << "Dot product = " << planeVector.dot(planeNormal) << std::endl;
        }
    }
}