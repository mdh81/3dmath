#include "gtest/gtest.h"
#include "3dmath/Vector.h"
#include <vector>
#include <initializer_list>
#include <iostream>
#include <fstream>
using namespace std;
using namespace math3d;

TEST(Vector, DefaultConstruction) {
    // Assert all vector entries are zeroes
    Vector<float, 10> v; 
    float const* data = v.getData();
    for (int i = 0; i < 10; ++i) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Vector, InitializerConstruction) {
    Vector<float, 4> v {1.0, 10.0, 20.0, 30.0};
    float const* data = v.getData();
    ASSERT_FLOAT_EQ(1.0, data[0]);
    ASSERT_FLOAT_EQ(10.0, data[1]);
    ASSERT_FLOAT_EQ(20.0, data[2]);
    ASSERT_FLOAT_EQ(30.0, data[3]);
    EXPECT_THROW(({
        try {
            Vector<float, 3> v {1.0, 10.0, 20.0, 30.0};
        } catch (std::invalid_argument& ex) {
            EXPECT_STREQ("Dimension mismatch: Vector's dimension is 3 "
                         "Input size is 4", ex.what()); 
            throw;
        }

    }), std::invalid_argument);
}

TEST(Vector, CopyConstruction) {
    Vector<float, 4> v {1.0, 10.0, 20.0, 30.0};
    Vector<float, 4> v2(v);
    float const* data = v2.getData();
    ASSERT_FLOAT_EQ(1.0, data[0]);
    ASSERT_FLOAT_EQ(10.0, data[1]);
    ASSERT_FLOAT_EQ(20.0, data[2]);
    ASSERT_FLOAT_EQ(30.0, data[3]);
    // NOTE: Compiler protects against passing an instance of Vector whose
    // dimensions are different to the copy constructor. Same for data type
}

TEST(Vector, CopyAssignment) {
    Vector<float, 4> v {1.0, 10.0, 20.0, 30.0};
    Vector<float, 4> v2;
    v2 = v;
    float const* data = v2.getData();
    ASSERT_FLOAT_EQ(1.0, data[0]);
    ASSERT_FLOAT_EQ(10.0, data[1]);
    ASSERT_FLOAT_EQ(20.0, data[2]);
    ASSERT_FLOAT_EQ(30.0, data[3]);
    float const* data1 = v.getData();
    ASSERT_NE(data1, data) << "Expected copy assignment to do a new allocation";
    ASSERT_FLOAT_EQ(1.0, data1[0]);
    ASSERT_FLOAT_EQ(10.0, data1[1]);
    ASSERT_FLOAT_EQ(20.0, data1[2]);
    ASSERT_FLOAT_EQ(30.0, data1[3]);

}

TEST(Vector, VectorAddition) {
    Vector<float, 3> v1({1,2,3});
    Vector<float, 3> v2({1,2,3});
    Vector<float, 3> v3 = v1 + v2;
    float const* data = v3.getData();
    ASSERT_FLOAT_EQ(2, data[0]);
    ASSERT_FLOAT_EQ(4, data[1]);
    ASSERT_FLOAT_EQ(6, data[2]);
    data = v1.getData();
    ASSERT_FLOAT_EQ(1, data[0]);
    ASSERT_FLOAT_EQ(2, data[1]);
    ASSERT_FLOAT_EQ(3, data[2]);
}

TEST(Vector, ComponentAccess) {
    Vector<float, 3> v1({1,2,3});
    ASSERT_FLOAT_EQ(v1[0],1);
    ASSERT_FLOAT_EQ(v1[1],2);
    ASSERT_FLOAT_EQ(v1[2],3);
    v1[0] = 3;
    v1[1] = 50;
    v1[2] = 1;
    ASSERT_FLOAT_EQ(v1[0],3);
    ASSERT_FLOAT_EQ(v1[1],50);
    ASSERT_FLOAT_EQ(v1[2],1);
}


TEST(Vector, CrossProduct) {
    Vector<float, 3> v1({5, 0, 0});
    Vector<float, 3> v2({0, 5, 0});
    Vector<float, 3> v3 = v1 * v2;
    ASSERT_FLOAT_EQ(v3[2], 25);
}

TEST(Vector, Length) {
    Vector<float, 3> v1({5, 0, 0});
    ASSERT_FLOAT_EQ(v1.length(), 5);
    Vector<float, 3> v2({0, 5, 0});
    ASSERT_FLOAT_EQ(v2.length(), 5);
    Vector<float, 3> v3 = v1 * v2;
    ASSERT_FLOAT_EQ(v3.length(), 25);
}

TEST(Vector, Normalize) {
    Vector<float, 3> v1({5, 0, 0});
    Vector<float, 3> v2({0, 5, 0});
    Vector<float, 3> v3 = v1 * v2;
    v3.normalize();
    ASSERT_FLOAT_EQ(v3.length(), 1);
}

TEST(Vector, Difference) {
    Vector<float, 3> v2({10, 0, 0});
    Vector<float, 3> v1({5, 0, 0});
    Vector<float, 3> v12v2 = v2 - v1;
    ASSERT_FLOAT_EQ(v12v2[0], 5);
    ASSERT_FLOAT_EQ(v12v2[1], 0);
    ASSERT_FLOAT_EQ(v12v2[2], 0);
}

TEST(Vector, Scale) {
    Vector<float, 3> v1({10, 20, 30});
    v1 /= 10;
    ASSERT_FLOAT_EQ(v1[0], 1);
    ASSERT_FLOAT_EQ(v1[1], 2);
    ASSERT_FLOAT_EQ(v1[2], 3);
}

TEST(Vector, AddToSelf) {
    Vector<float, 3> v1({10, 20, 30});
    Vector<float, 3> v2({10, 20, 30});
    v1 += v2;
    ASSERT_FLOAT_EQ(v1[0], 20);
    ASSERT_FLOAT_EQ(v1[1], 40);
    ASSERT_FLOAT_EQ(v1[2], 60);
    ASSERT_FLOAT_EQ(v2[0], 10);
    ASSERT_FLOAT_EQ(v2[1], 20);
    ASSERT_FLOAT_EQ(v2[2], 30);
}

TEST(Vector, DotProduct) {
    Vector<float, 3> v1({5, 0, 0});
    Vector<float, 3> v2({5, 0, 0});
    ASSERT_FLOAT_EQ(v1.dot(v2), 25);
    Vector<float, 3> v3({0, -5, 0});
    ASSERT_FLOAT_EQ(v1.dot(v3), 0);
}

TEST(Vector, BuildFromSTLVector) {
    std::vector<float> v(5, 8.3f);
    Vector<float, 5> v1(v);
    for(auto i : {0,1,2,3,4})
        ASSERT_FLOAT_EQ(v[i], 8.3f);
}

TEST(Vector2D, EmplaceConstructor) {
    std::vector<Point2D<float>> points;
    points.emplace_back(0.f, 1.f);
    ASSERT_FLOAT_EQ(points[0].x, 0.f);
    ASSERT_FLOAT_EQ(points[0].y, 1.f);
}

TEST(Vector3D, EmplaceConstructor) {
    std::vector<Point3D<float>> points;
    points.emplace_back(0.f, 1.f, 2.f);
    ASSERT_FLOAT_EQ(points[0].x, 0.f);
    ASSERT_FLOAT_EQ(points[0].y, 1.f);
    ASSERT_FLOAT_EQ(points[0].z, 2.f);
}
