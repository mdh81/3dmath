#include "gtest/gtest.h"
#include "3dmath/MatrixOperations.h"
#include "3dmath/IdentityMatrix.h"
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
    auto transposedMatrix = matrix.transpose();
    using Column = Vector<float, 3>;
    Column column0 = transposedMatrix[0];
    ASSERT_FLOAT_EQ(column0[0], 1);
    ASSERT_FLOAT_EQ(column0[1], 4);
    ASSERT_FLOAT_EQ(column0[2], 7);

    Column column1 = transposedMatrix[1];
    ASSERT_FLOAT_EQ(column1[0], 2);
    ASSERT_FLOAT_EQ(column1[1], 5);
    ASSERT_FLOAT_EQ(column1[2], 8);

    Column column2 = transposedMatrix[2];
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

TEST(MatrixOperations, DeterminantOfValidMatrices) {
    Matrix<float, 3, 3> testMatrix {{2.f, 1.f, 3.f},
                                    {-3.f, -1.f, 2.f},
                                    {1.f, 2.f, 4.f}};
    auto result = testMatrix.determinant();
    ASSERT_FLOAT_EQ(-17, result);

    auto identityMatrix = IdentityMatrix<float, 3, 3>{};
    result = identityMatrix.determinant();
    ASSERT_FLOAT_EQ(1.f, result);
}

TEST(MatrixOperations, DeterminantOfMatrixWithZerosInLastColumn) {
    Matrix<float, 2, 2> testMatrix {{{1,2},{0,0}}, Order::ColumnMajor};
    ASSERT_FLOAT_EQ(0, testMatrix.determinant());
}

TEST(MatrixOperations, DeterminantOfMatrixWithAllZerosInColumns) {
    Matrix<float, 3, 3> testMatrix {{{1,2,3},{0,0,0},{4,5,6}}, Order::ColumnMajor};
    std::string errorMessage;
    try {
        testMatrix.determinant();
    } catch(std::exception& ex) {
        errorMessage = ex.what();
    }
    ASSERT_EQ(errorMessage, "Matrix::determinant(): Invalid matrix. Column 1 has all zeroes");
}