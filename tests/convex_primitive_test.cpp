#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "3dmath/Vector.h"
#include "3dmath/OtherTypes.h"
#include "3dmath/primitives/ConvexPrimitive.h"
#include "3dmath/primitives/Sphere.h"
using namespace std;

class ConvexPrimitiveTest : public math3d::ConvexPrimitive {
    public:
        explicit ConvexPrimitiveTest(Point const& center) : ConvexPrimitive(center) {
        }
        void setVertices(Primitive::Vertices& testVertices) {
            vertices = testVertices;
        }
        MOCK_METHOD(void, generateGeometry, ());
        MOCK_METHOD(Point, intersectWithRay, (math3d::Ray const&));
};

TEST(ConvexPrimitive, OrientNormalsOutside) {
    ConvexPrimitiveTest convexPrimitive({0, 0, 0});
    math3d::Primitive::Vertices vertices;
    vertices.push_back({-5.f, -5.f, +5.f}); // NOLINT: Don't care about push_back inefficiency in test code
    vertices.push_back({+5.f, -5.f, +5.f}); // NOLINT
    vertices.push_back({+0.f, +5.f, +5.f}); // NOLINT
    convexPrimitive.setVertices(vertices);
    math3d::ConvexPrimitive::Tri t1 = convexPrimitive.orientTriangleNormalOutside({0, 2, 1});
    ASSERT_EQ(std::get<0>(t1), 0) << "Normal was not oriented outside";
    ASSERT_EQ(std::get<1>(t1), 1) << "Normal was not oriented outside";
    ASSERT_EQ(std::get<2>(t1), 2) << "Normal was not oriented outside";

    math3d::ConvexPrimitive::Tri t2 = convexPrimitive.orientTriangleNormalOutside({0, 1, 2});
    ASSERT_EQ(std::get<0>(t2), 0) << "Normal was incorrectly oriented inside";
    ASSERT_EQ(std::get<1>(t2), 1) << "Normal was incorrectly oriented inside";
    ASSERT_EQ(std::get<2>(t2), 2) << "Normal was incorrectly oriented inside";
}
