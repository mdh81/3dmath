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
    cout << "Before swap\n" << testMatrix << endl;
    auto result = matrixTestWrapper.swapRows(0, 1);
    cout << "After swap\n" << result << endl;
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

    Matrix<float, 2, 2> testMatrix2 {{
        {15,5},
        {2,10}
    }};
    result = testMatrix2.determinant();
    ASSERT_FLOAT_EQ(15*10 - 2*5, result) << "2x2 determinant is not the difference between product of diagonal elements";
}

TEST(MatrixOperations, DeterminantOfMatrixWithZerosInLastColumn) {
    Matrix<float, 2, 2> testMatrix {{
        {1,0},
        {2,0}
    }};
    ASSERT_FLOAT_EQ(0, testMatrix.determinant());
}

TEST(MatrixOperations, DeterminantOfMatrixWithAllZerosInColumns) {
    Matrix<float, 3, 3> testMatrix {{
        {1,0,4},
        {2,0,5},
        {3,0,6}
    }};
    std::string errorMessage;
    try {
        testMatrix.determinant();
    } catch(std::exception& ex) {
        errorMessage = ex.what();
    }
    ASSERT_EQ(errorMessage, "Matrix::determinant(): Invalid matrix. Column 1 has all zeroes");
}

TEST(MatrixOperations, InverseOfNonInvertibleMatrix) {
    Matrix<float, 3, 3> testMatrix {{
        {1,0,4},
        {2,0,5},
        {3,0,6}
    }};

    std::string errorMessage;
    try {
        testMatrix.inverse();
    } catch(std::exception& ex) {
        errorMessage = ex.what();
    }
    ASSERT_EQ(errorMessage, "Matrix is not invertible. Pivot elements are zero");
}

TEST(MatrixOperations, Inverse) {
    Matrix<float, 3, 3> testMatrix {{
        {1,4,7},
        {2,5,8},
        {3,6,11}
    }};

    auto inverse = testMatrix.inverse();

    // TODO
    // 1. Why is ASSERT_FLOAT_EQ failing
    // 2. Write a robust floating point comparison function
    ASSERT_EQ("  -1.16667   0.333333        0.5\n"
              " -0.333333    1.66667         -1\n"
              "       0.5         -1        0.5\n", inverse.asString());
}