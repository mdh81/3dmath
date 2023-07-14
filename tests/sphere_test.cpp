#include "gtest/gtest.h"
#include "3dmath/Vector.h"
#include "TestSupport.h"
#include "3dmath/primitives/Sphere.h"
#include "3dmath/primitives/Ray.h"
#include <vector>

TEST(Sphere, Getters) {
    math3d::Sphere sphere({10.f, 10.f, 10.f}, 10.f);
    ASSERT_FLOAT_EQ(sphere.getRadius(), 10) << "Sphere radius is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().x, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().y, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().z, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getResolution(), 16) << "Sphere resolution is wrong";
}


TEST(Sphere, GeometryGenerationVertices) {
    math3d::Sphere sphere({0.f, 0.f, 0.f}, 10.f);
    sphere.generateGeometry();
    auto resolution = sphere.getResolution();
    ASSERT_EQ(sphere.getVertices().size(), resolution * (resolution - 1) + 2) << "Number of vertices in the sphere is wrong";
}

TEST(Sphere, GeometryGenerationConnectivity) {
    math3d::Sphere sphere({10.f, 10.f, 10.f}, 10.f, 3);
    sphere.generateGeometry();
    auto resolution = sphere.getResolution();
    auto numCircles = resolution - 1;
    auto numQuadsBetweenTwoCircles = resolution;
    auto numTrisPoles = 2 * resolution;
    auto numTris =  numTrisPoles + ((numCircles - 1) * numQuadsBetweenTwoCircles * 2);
    ASSERT_EQ(sphere.getTris().size(), numTris) << "Number of triangles in the sphere is wrong";
    auto vertexPole1 = std::get<0>(sphere.getTris().at(0));
    auto vertexPole2= std::get<2>(sphere.getTris().at(numTris-1));
    std::vector<unsigned> verticesBetweenPole1AndCircle1{0, 1, 2, 3};
    ASSERT_TRUE(std::any_of(verticesBetweenPole1AndCircle1.begin(), verticesBetweenPole1AndCircle1.end(),
                            [vertexPole1](auto elem) { return elem == vertexPole1; })) << "Pole 1 vertex is wrong";
    std::vector<unsigned> verticesBetweenPole2AndCircle2{4, 5, 6, 7};
    ASSERT_TRUE(std::any_of(verticesBetweenPole2AndCircle2.begin(), verticesBetweenPole2AndCircle2.end(),
                            [vertexPole2](auto elem) { return elem == vertexPole2; })) << "Pole 2 vertex is wrong";
    std::remove_reference_t<decltype(sphere.getTris())> trisBetweenCircle1AndCircle2(sphere.getTris().begin() + 3, sphere.getTris().begin() + 9);
    std::vector<unsigned> verticesBetweenCircle1AndCircle2{1, 2, 3, 4, 5, 6};
    for (auto& tri : trisBetweenCircle1AndCircle2) {
        ASSERT_TRUE(std::any_of(verticesBetweenCircle1AndCircle2.begin(), verticesBetweenCircle1AndCircle2.end(),
                                [&tri](auto vertexIndex) { return std::get<0>(tri) == vertexIndex; }));
        ASSERT_TRUE(std::any_of(verticesBetweenCircle1AndCircle2.begin(), verticesBetweenCircle1AndCircle2.end(),
                                [&tri](auto vertexIndex) { return std::get<1>(tri) == vertexIndex; }));
        ASSERT_TRUE(std::any_of(verticesBetweenCircle1AndCircle2.begin(), verticesBetweenCircle1AndCircle2.end(),
                                [&tri](auto vertexIndex) { return std::get<2>(tri) == vertexIndex; }));
    }
}

TEST(Sphere, STLOutput) {
    auto outputPath = math3d::test::TestSupport::getOutputDirectory() / "Sphere.stl";
    auto baselinePath = math3d::test::TestSupport::getBaselineDirectory() / "Sphere.stl";
    math3d::Sphere {{10, 10, 10}, 10, 16}.writeToFile(outputPath);
    ASSERT_TRUE(
            math3d::test::TestSupport::areBinarySTLFilesEqual(
                    outputPath, baselinePath)) << "Geometry in STL file is different";
}

TEST(Sphere, OBJOutput) {
    auto outputPath = math3d::test::TestSupport::getOutputDirectory() / "Sphere.obj";
    math3d::Sphere {{10, 10, 10}, 10, 16}.writeToFile(outputPath);
}

TEST(Sphere, RayIntersection) {
    // Ray origin inside the sphere
    auto intersectionResult1 = math3d::Sphere {{10, 10, 10}, 10 }.intersectWithRay(
            math3d::Ray{ {10, 10, 10}, {1, 0, 0}});
    ASSERT_EQ(intersectionResult1.status, math3d::IntersectionStatus::Intersects);
    ASSERT_FLOAT_EQ(intersectionResult1.intersectionPoint.x, 10);
    ASSERT_FLOAT_EQ(intersectionResult1.intersectionPoint.y, 10);
    ASSERT_FLOAT_EQ(intersectionResult1.intersectionPoint.z, 10);

    // Ray origin outside the sphere and no intersection
    auto intersectionResult2 = math3d::Sphere {{10, 10, 10}, 10 }.intersectWithRay(
            math3d::Ray{ {0, 0, 0}, {1, 0, 0}});
    ASSERT_EQ(intersectionResult2.status, math3d::IntersectionStatus::NoIntersection);

    // Ray origin outside the sphere and there is an intersection
    auto intersectionResult3 = math3d::Sphere {{10, 0, 0}, 1 }.intersectWithRay(
            math3d::Ray{ {0, 0, 0}, {1, 0, 0}});
    ASSERT_EQ(intersectionResult3.status, math3d::IntersectionStatus::Intersects);
    ASSERT_FLOAT_EQ(intersectionResult3.intersectionPoint.x, 10);
    ASSERT_FLOAT_EQ(intersectionResult3.intersectionPoint.y, 0);
    ASSERT_FLOAT_EQ(intersectionResult3.intersectionPoint.z, 0);
}

TEST(Sphere, RayIntersectionRobustness) {

    // Construct rays whose origin is within the sphere. All of those rays should intersect
    // and the intersection point must be on the sphere
    double sphereRadius = math3d::Utilities::RandomNumber();
    math3d::types::Point3D sphereCenter = math3d::Utilities::RandomVector();
    auto sphere = math3d::Sphere(sphereCenter, sphereRadius);
    auto numSamples = 100;
    for (auto i = 0; i < numSamples; ++i) {
        math3d::types::Vector3D rayDirection = math3d::Utilities::RandomVector();
        rayDirection.normalize();
        auto ray = math3d::Ray(sphereCenter + rayDirection * math3d::Utilities::RandomNumber(0, sphereRadius), rayDirection);
        auto result = sphere.intersectWithRay(ray);
        ASSERT_EQ(result.status, math3d::IntersectionStatus::Intersects);
        auto rayOriginToSphereCenter = sphereCenter - ray.getOrigin();
        bool rayOriginBehindSphereCenter = (sphereCenter - ray.getOrigin()).dot(ray.getDirection()) < 0;
        math3d::types::Point3D expectedIntersectionPoint;
        if (rayOriginBehindSphereCenter) {
            expectedIntersectionPoint = sphereCenter - (sphereRadius * rayDirection);
        } else {
            expectedIntersectionPoint = sphereCenter + (sphereRadius * rayDirection);
        }
        ASSERT_FLOAT_EQ((result.intersectionPoint - expectedIntersectionPoint).lengthSquared(), 0);
    }

}

