#include "gtest/gtest.h"
#include "3dmath/Matrix.h"
#include <vector>
using namespace std;
using namespace math3d;

namespace math3d {
    // Friend class of matrix to get results of private methods
    template<size_t M, size_t N>
    class MatrixTestWrapper {
    public:
        explicit MatrixTestWrapper(Matrix<float,M,N> const& testMatrix) : testMatrix(testMatrix) {}

        auto getMinor(int i , int j) {
            return testMatrix.getMinor(i, j);
        }

    private:
        Matrix<float, M, N> testMatrix;
    };
}

TEST(MatrixInverse, Minor) {
    Matrix<float, 3, 3> matrix;
    MatrixTestWrapper<3, 3> matrixTest(matrix);
    matrixTest.getMinor(0, 0);

}