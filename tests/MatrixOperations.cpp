#include "gtest/gtest.h"
#include "3dmath/MatrixOperations.h"
#include <vector>
using namespace std;
using namespace math3d;

namespace math3d {
    // Friend class of matrix to get results of private methods
    template<size_t M, size_t N>
    class MatrixTestWrapper {
    public:
        explicit MatrixTestWrapper(Matrix<float,M,N> const& testMatrix) : testMatrix(testMatrix) {}

        auto getMinor(size_t i , size_t j) {
            return testMatrix.getMinor(i, j);
        }

        auto swapRows(size_t i, size_t j) {
            testMatrix.swapRows(i, j);
            return decltype(testMatrix)(testMatrix);
        }

    private:
        Matrix<float, M, N> testMatrix;
    };

}

TEST(MatrixOperations, Transpose) {
    // Inner init list is a row since the default order is row major
    Matrix<float, 3, 3> matrix {
            {1, 4, 7},
            {2, 5, 8},
            {3, 6, 9} };
    auto const transposedMatrix = matrix.transpose();
    auto column0 = transposedMatrix.getColumn(0);
    ASSERT_FLOAT_EQ(column0[0], 1);
    ASSERT_FLOAT_EQ(column0[1], 4);
    ASSERT_FLOAT_EQ(column0[2], 7);

    auto column1 = transposedMatrix.getColumn(1);
    ASSERT_FLOAT_EQ(column1[0], 2);
    ASSERT_FLOAT_EQ(column1[1], 5);
    ASSERT_FLOAT_EQ(column1[2], 8);

    auto column2 = transposedMatrix.getColumn(2);
    ASSERT_FLOAT_EQ(column2[0], 3);
    ASSERT_FLOAT_EQ(column2[1], 6);
    ASSERT_FLOAT_EQ(column2[2], 9);
}

TEST(MatrixOperations, SwapRows) {
    Matrix<float, 3, 3> testMatrix {{2.f, 1.f, 3.f},
                                    {-3.f, -1.f, 2.f},
                                    {1.f, 2.f, 4.f}};
    MatrixTestWrapper<3, 3> matrixTestWrapper(testMatrix);
    auto result = matrixTestWrapper.swapRows(0, 1);
    ASSERT_FLOAT_EQ(result(0, 0), -3.f);
    ASSERT_FLOAT_EQ(result(0, 1), -1.f);
    ASSERT_FLOAT_EQ(result(0, 2), +2.f);
    ASSERT_FLOAT_EQ(result(1, 0), +2.f);
    ASSERT_FLOAT_EQ(result(1, 1), +1.f);
    ASSERT_FLOAT_EQ(result(1, 2), +3.f);
}

TEST(MatrixOperations, Determinant) {
    Matrix<float, 3, 3> testMatrix {{2.f, 1.f, 3.f},
                                    {-3.f, -1.f, 2.f},
                                    {1.f, 2.f, 4.f}};
    auto result = testMatrix.determinant();
    ASSERT_FLOAT_EQ(-17, result);
}

TEST(MatrixOperations, Minor3x3) {
    Matrix<float, 3, 3> matrix {{10.f, 0.f, 0.f}, {0.f, 20.f, 0.f}, {0.f, 0.f, 30.f}};
    MatrixTestWrapper<3, 3> matrixTest(matrix);
}