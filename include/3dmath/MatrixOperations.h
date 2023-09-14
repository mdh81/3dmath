#pragma once

#include "Matrix.h"
#include <limits>
#include <algorithm>
#include <iostream>

// TODO: Unify all exceptions such that they include the method name as the prefix in the error message
namespace math3d {
    template<typename DataType, size_t numRows, size_t numCols>
    DataType Matrix<DataType, numRows, numCols>::determinant() {

        static_assert(numRows == numCols, "Matrix::determinant(): Determinants are not defined for non-square matrices");
        static_assert(std::is_floating_point<DataType>::value, "Matrix::determinant(): Matrix's data type needs to be floating point");

        // Calculate determinant using the Gaussian elimination with partial pivoting
        char sign = 1;
        Matrix upperTriangular(*this);

        // NOTE: Last column skipped because there are no sub-diagonal elements in the last column
        for (size_t columnIndex = 0; columnIndex < numCols-1; ++columnIndex) {

            // Step 1: Pick a pivot element
            Vector<DataType, numRows> column = upperTriangular[columnIndex];
            size_t pivotIndex {};
            DataType pivotValue = column[0];
            for (size_t rowIndex = 1; rowIndex < numRows; ++rowIndex) {
                if (abs(column[rowIndex]) > abs(pivotValue)) {
                    pivotIndex = rowIndex;
                    pivotValue = column[pivotIndex];
                }
            }
            if (fabs(pivotValue) < constants::tolerance) {
                throw std::runtime_error("Matrix::determinant(): Invalid matrix. Column " +
                                         std::to_string(columnIndex) + " has all zeroes");
            }

            // Step 2: Swap rows to make the pivot element the element with the largest absolute value
            if (pivotIndex != columnIndex) {
                upperTriangular.swapRows(pivotIndex, columnIndex);
                pivotIndex = columnIndex;
                sign *= -1;
            }

            // Step 3: Eliminate sub-diagonal elements by
            // a) multiplying the pivot row by matrix(rowIndex, columnIndex) / pivotValue
            // b) subtracting current row from the pivot row multiplied by the pivot value
            DataType factor {};
            for (size_t rowIndex = columnIndex+1; rowIndex < numRows; ++rowIndex) {
                factor = upperTriangular(rowIndex, columnIndex) / pivotValue;
                upperTriangular.addRow(rowIndex, upperTriangular(pivotIndex) * -factor);
            }
        }

        // Step 4: Multiply elements on the main diagonal to compute the determinant. Set the sign based on the
        // number of row swaps
        DataType result {1.f};
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (i == j) result *= upperTriangular(i, j);
            }
        }
        return result * sign;
    }

    template<typename DataType, size_t numRows, size_t numCols>
    Matrix<DataType, numRows, numCols> Matrix<DataType, numRows, numCols>::transpose() {
        Matrix <DataType, numCols, numRows> transposedMatrix;
        auto& transposedData = transposedMatrix.data;
        for (auto row = 0u; row < numRows; ++row) {
            for (auto col = 0u; col < numCols; ++col) {
                transposedData[row * numCols + col] = data[col * numRows + row];
            }
        }
        return transposedMatrix;
    }


    template<typename DataType, size_t numRows, size_t numCols>
    void Matrix<DataType, numRows, numCols>::swapRows(size_t rowA, size_t rowB) {
        if (rowA >= numRows || rowB >= numRows) {
            throw std::runtime_error("Invalid row index encountered in swap call");
        }
        DataType tmp;
        for (size_t i = 0; i < numCols; ++i) {
            tmp = data[rowB];
            data[rowB] = data[rowA];
            data[rowA] = tmp;
            rowB += numCols;
            rowA += numCols;
        }
    }

    template<typename DataType, size_t numRows, size_t numCols>
    void Matrix<DataType, numRows, numCols>::addRow(size_t rowIndex, Vector<DataType, numCols> const& anotherRow) {
        if (rowIndex >= numRows) {
            throw std::runtime_error("Matrix::addRow(): Invalid row index!");
        }
        for (size_t i = 0, index = rowIndex; i < numCols; ++i, index += numRows) {
           data[index] += anotherRow[i];
        }
    }
}