#include "gtest/gtest.h"
#include "3dmath/Vector.h"
#include "TestSupport.h"
#include "3dmath/primitives/Sphere.h"
#include <vector>

class SphereTest : public math3d::Sphere {
public:
    explicit SphereTest(Point const& center, float radius, unsigned resolution = 16)
        : Sphere(center, radius, resolution) {
    }
};

TEST(Sphere, Getters) {
    SphereTest sphere({10.f, 10.f, 10.f}, 10.f);
    ASSERT_FLOAT_EQ(sphere.getRadius(), 10) << "Sphere radius is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().x, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().y, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getCenter().z, 10.f) << "Sphere center is incorrect";
    ASSERT_FLOAT_EQ(sphere.getResolution(), 16) << "Sphere resolution is wrong";
}


TEST(Sphere, GeometryGenerationVertices) {
    SphereTest sphere({0.f, 0.f, 0.f}, 10.f);
    sphere.generateGeometry();
    auto resolution = sphere.getResolution();
    ASSERT_EQ(sphere.getVertices().size(), resolution * (resolution - 1) + 2) << "Number of vertices in the sphere is wrong";
    sphere.writeToSTL("S10.stl");
}

TEST(Sphere, GeometryGenerationConnectivity) {
    SphereTest sphere({10.f, 10.f, 10.f}, 10.f, 3);
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

TEST(Sphere, WriteToSTL) {
    // TODO: Unreliable because of floating point precision differences. Find a better
    //  way to compare geometries
    math3d::Sphere {{10, 10, 10}, 10, 16}.writeToSTL("Sphere.stl");
    ASSERT_TRUE(
            math3d::test::TestSupport::areFilesEqual(
                    "Sphere.stl",
                    std::filesystem::path("baseline")/"Sphere.stl")) << "Geometry in STL file is different";
}

