#include "gtest/gtest.h"
#include "3dmath/Matrix.h"
#include <vector>
#include <initializer_list>
#include <iostream>
using namespace std;
using namespace math3d;

TEST(Matrix, TestDefaultConstruction) {
    // Assert all matrix entries are zeroes
    Matrix<float, 10, 10> m; 
    ASSERT_EQ(m.getNumberOfRows(), 10);
    ASSERT_EQ(m.getNumberOfColumns(), 10);
    float const* data = m.getData();
    for (int i = 0; i < 99; ++i) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, TestIdentity) {
    
    // Assert identity matrix is a diagonal matrix
    IdentityMatrix<float, 3, 3> m1; 
    float const* data = m1.getData();
    for (auto i : {0,4,8}) {
        ASSERT_FLOAT_EQ(1, data[i]);
    }
    for (auto i : {1,2,3,5,6,7}) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, TestCopyConstruction) {
    
    IdentityMatrix<float, 3, 3>m1;
    IdentityMatrix<float, 3, 3>m2(m1);
    float const* data = m2.getData();
    for (auto i : {0,4,8}) {
        ASSERT_FLOAT_EQ(1, data[i]);
    }
    for (auto i : {1,2,3,5,6,7}) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, TestInitialization) {
    
    // Dimensions don't match: Columns don't match
    EXPECT_THROW(({
        try {
            Matrix<float, 2, 1> m2({{1.f}, {2.f}, {3.f}});
        } catch (std::invalid_argument& ex) {
            EXPECT_STREQ("Incompatible dimensions: Matrix dimensions are [2,1] "
                         "Outer initializer list's size is 3", ex.what()); 
            throw;
        }
    }), std::invalid_argument);

    // Dimensions don't match: Rows don't match
    EXPECT_THROW(({
        try {
            Matrix<float, 1, 2> m2({{1.f}, {2.f, 3.f}});
        } catch (std::invalid_argument& ex) {
            EXPECT_STREQ("Incompatible dimensions: Matrix dimensions are [1,2] "
                         "One of the inner initializer list's size is 2", ex.what()); 
            throw;
        }
    }), std::invalid_argument);
    
    // Assert column major matrix is filled correctly 
    Matrix<int, 2, 2> m3 {{1,2},{3,4}};
    int const* data = m3.getData();
    ASSERT_EQ(data[0], 1);
    ASSERT_EQ(data[1], 2);
    ASSERT_EQ(data[2], 3);
    ASSERT_EQ(data[3], 4);

}

TEST(Matrix, TestCopyAssignment) {
    Matrix<float, 4, 5> m1;
}
