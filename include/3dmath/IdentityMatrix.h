#pragma once
#include "Matrix.h"
namespace math3d {

template<typename DataType, size_t numRows, size_t numCols> requires (numRows == numCols)
class IdentityMatrix : public Matrix<DataType, numRows, numCols> {
protected:
    using Matrix<DataType, numRows, numCols>::data;
public:
    IdentityMatrix() {
        for (size_t col = 0; col < numCols; ++col) {
            for (size_t row = 0; row < numRows; ++row) {
                if (row == col) {
                    data[col * numRows + row] = DataType{1};
                }
            }
        }
    }
};

}
