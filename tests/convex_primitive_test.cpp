#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "3dmath/Vector.h"
#include "3dmath/OtherTypes.h"
#include "3dmath/primitives/ConvexPrimitive.h"
#include <vector>
using namespace std;
using namespace math3d;

namespace math3d {
    class MockConvexPrimitive : public ConvexPrimitive {
    public:
        explicit MockConvexPrimitive(Point const& center) : ConvexPrimitive(center) {
        }
        void setVertices(Primitive::Vertices& testVertices) {
            vertices = testVertices;
        }
        MOCK_METHOD(void, generateGeometry, ());
        MOCK_METHOD(Point, intersectWithRay, (Ray const&));
    };
}

TEST(ConvexPrimitive, OrientNormalsOutside) {
    MockConvexPrimitive convexPrimitive({0, 0, 0});
    Primitive::Vertices vertices;
    vertices.push_back({-5.f, -5.f, +5.f}); // NOLINT
    vertices.push_back({+5.f, -5.f, +5.f}); // NOLINT
    vertices.push_back({+0.f, +5.f, +5.f}); // NOLINT
    convexPrimitive.setVertices(vertices);
    ConvexPrimitive::Tri t1{0, 2, 1};
    convexPrimitive.orientTriangleNormalOutside(t1);
    ASSERT_EQ(std::get<0>(t1), 0) << "Normal was not oriented outside";
    ASSERT_EQ(std::get<1>(t1), 1) << "Normal was not oriented outside";
    ASSERT_EQ(std::get<2>(t1), 2) << "Normal was not oriented outside";

    ConvexPrimitive::Tri t2{0, 1, 2};
    convexPrimitive.orientTriangleNormalOutside(t1);
    ASSERT_EQ(std::get<0>(t2), 0) << "Normal was incorrectly oriented inside";
    ASSERT_EQ(std::get<1>(t2), 1) << "Normal was incorrectly oriented inside";
    ASSERT_EQ(std::get<2>(t2), 2) << "Normal was incorrectly oriented inside";
}
