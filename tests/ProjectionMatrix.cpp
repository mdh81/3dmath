#include "gtest/gtest.h"
#include "3dmath/ProjectionMatrix.h"
#include "3dmath/OrthographicProjectionMatrix.h"
#include <iostream>
using namespace std;
using namespace math3d;

TEST(ProjectionMatrix, OrthographicProjectionMatrix) {
    auto projectionMatrix = OrthographicProjectionMatrix<float>();
    projectionMatrix.update({{-1.f, -1.f, -1.f}, {1.f, 1.f, 1.f}});
    auto data = projectionMatrix.getData();
    // Assert that z is inverted and the rest of the matrix is identity
    ASSERT_FLOAT_EQ(data[0],  1.f);
    ASSERT_FLOAT_EQ(data[5],  1.f);
    ASSERT_FLOAT_EQ(data[10], -1.f);
    ASSERT_FLOAT_EQ(data[15], 1.f);
    for (auto index : {1, 2, 3, 4, 6, 7, 8, 9, 11, 12, 13, 14}) {
        ASSERT_FLOAT_EQ(data[index], 0.f);
    }
}

TEST(ProjectionMatrix, OrthographicProjectionMatrixWithInvalidBounds) {
    EXPECT_THROW({
         try {
             [[maybe_unused]]
             auto orthoMatrix = OrthographicProjectionMatrix<float>({{0,0}, {0,0}, {0,0}});
         } catch (std::runtime_error &ex) {
             throw;
         }
    }, std::runtime_error) << "Expected a runtime error when invalid bounds are specified";
}