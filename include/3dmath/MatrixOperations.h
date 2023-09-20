#pragma once

#include "Matrix.h"
#include "IdentityMatrix.h"
#include "Utilities.h"
#include <limits>
#include <algorithm>
#include <iostream>

namespace math3d {

    // Convert a square matrix or the square matrix portion of an augmented matrix to an upper triangular matrix
    template<typename DataType, size_t numRows, size_t numCols>
    unsigned Matrix<DataType, numRows, numCols>::convertToUpperTriangular(Matrix<DataType, numRows, numCols>& upperTriangular) const {

        // Convert to upper triangular using Gaussian elimination with partial pivoting
        upperTriangular = *this;

        auto numRowSwaps = 0u;

        // NOTE 1: Rectangular matrices cannot be triangular. The loop assumes numRows = numColumns, but
        //         it uses numRows in the loop termination condition because we want to support turning
        //         an augmented matrix, which is rectangular into a triangular matrix by ONLY considering
        //         the non-augmented part, which is of dimension numRows x numRows
        // NOTE 2: Last column skipped because there are no sub-diagonal elements in the last column to be
        //         turned into zeroes
        constexpr size_t numberOfColumns = numRows - 1;
        for (size_t columnIndex = 0; columnIndex < numberOfColumns; ++columnIndex) {

            // Step 1: Pick a pivot element
            // This is the element with the largest absolute value in the current column
            // below the current row (which is also the current column since we are dealing with square matrices)
            Vector<DataType, numRows> column = upperTriangular[columnIndex];
            size_t pivotIndex {};
            DataType pivotValue = column[columnIndex];
            for (size_t rowIndex = 1; rowIndex < numRows; ++rowIndex) {
                if (abs(column[rowIndex]) > abs(pivotValue)) {
                    pivotIndex = rowIndex;
                    pivotValue = column[pivotIndex];
                }
            }
            // If a column has all zeros there is no further operation to be done to transform that column
            if (fabs(pivotValue) < constants::tolerance) {
               continue;
            }

            // Step 2: Swap rows to make the pivot element the element with the largest absolute value
            if (pivotIndex != columnIndex) {
                upperTriangular.swapRows(pivotIndex, columnIndex);
                pivotIndex = columnIndex;
                ++numRowSwaps;
            }

            // Step 3: Eliminate sub-diagonal elements by
            // a) multiplying the pivot row by matrix(rowIndex, columnIndex) / pivotValue
            // b) subtracting current row from the pivot row multiplied by the pivot value
            DataType factor {};
            for (size_t rowIndex = columnIndex+1; rowIndex < numRows; ++rowIndex) {
                factor = upperTriangular(rowIndex, columnIndex) / pivotValue;
                upperTriangular.subtractRow(rowIndex, upperTriangular(pivotIndex) * factor);
            }
        }

        return numRowSwaps;
    }

    template<typename DataType, size_t numRows, size_t numCols>
    DataType Matrix<DataType, numRows, numCols>::determinant() {
        static_assert(numRows == numCols, "Matrix::determinant(): Determinants are not defined for non-square matrices");
        static_assert(std::is_floating_point<DataType>::value, "Matrix::determinant(): Matrix's data type needs to be floating point");

        // Calculate determinant using the Gaussian elimination with partial pivoting
        Matrix upperTriangular;
        auto numRowSwaps = convertToUpperTriangular(upperTriangular);

        // Multiply elements on the main diagonal  of the upper triangular matrix to compute the determinant.
        // Set the sign based on the number of row swaps
        DataType result {1.f};
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (i == j) result *= upperTriangular(i, j);
            }
        }
        return result * pow(-1, numRowSwaps);
    }

    template<typename DataType, size_t numRows, size_t numCols>
    Matrix<DataType, numRows, numCols> Matrix<DataType, numRows, numCols>::inverse() {
        static_assert(numRows == numCols, "Matrix::inverse(): Inverse is not defined. Only square matrices are invertible");
        static_assert(std::is_floating_point<DataType>::value,
                      "Matrix::determinant(): Matrix's data type needs to be floating point");

        // Compute inverse using Gauss-Jordan elimination

        // Step 1: Create augmented matrix
        // The augmented part of the matrix is an identity matrix, which gets converted to the actual inverse
        // in subsequent steps
        AugmentedMatrix<DataType, numRows, 2 * numCols, numCols> augmentedMatrix(*this, IdentityMatrix<DataType, numRows, numCols>{});

        // Step 2: Convert augmented matrix to upper triangular matrix
        Matrix<DataType, numRows, 2 * numCols> upperTriangular;
        augmentedMatrix.convertToUpperTriangular(upperTriangular);

        // Step 3: Check pivot elements for zeroes
        for (size_t rowIndex = 0; rowIndex < numRows; ++rowIndex) {
            DataType pivotValue = upperTriangular.data[rowIndex * numRows + rowIndex];
            if (Utilities::isZero(pivotValue)) {
                throw std::runtime_error("\nMatrix is not invertible. Zero found in a pivot location in the upper triangular form\n"
                                         + upperTriangular.asString());
            }
        }

        // Step 4: Convert non-pivot elements in the upper triangular matrix to 0
        // NOTE 1: In an upper-triangular matrix, elements A(i,j) = 0 when i > j. These are elements
        //         below the diagonal. Therefore, non-zero non-pivot elements are in columns [1, numRows)
        // NOTE 2: We use numRows instead of numCols for determining column index because we are dealing
        //         with a rectangular augmented matrix whose square portion is of size numRows x numRows
        for (size_t columnIndex = numRows-1; columnIndex > 0; --columnIndex) {
            for (size_t rowIndex = 0; rowIndex < numRows - 1; ++rowIndex) {
                if (rowIndex >= columnIndex) {
                    continue;
                }
                DataType factor = upperTriangular.data[columnIndex * numRows + rowIndex] /
                                  upperTriangular.data[columnIndex * numRows + rowIndex + 1];
                auto factorTimesNextRow = factor * upperTriangular(rowIndex + 1);
                upperTriangular.subtractRow(rowIndex, factorTimesNextRow);
            }
        }

        // Step 5: Divide each row by the pivot element
        // NOTE: Pivot elements are only present in the square matrix portion of the augmented matrix, which is of
        //       dimension numRows x numRows
        for (size_t rowIndex = 0; rowIndex < numRows; ++rowIndex) {
            auto pivotValue = upperTriangular.data[rowIndex * numRows + rowIndex];
            for (size_t columnIndex = 0; columnIndex < 2 * numRows; ++columnIndex) {
                upperTriangular.data[columnIndex * numRows + rowIndex] /= pivotValue;
            }
        }

        // Step 6: Return the augmented part of the matrix as the inverse
        return upperTriangular.template extract<numRows, numCols>(0, numCols);
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
            throw std::runtime_error("Matrix::swapRows(): Invalid row index encountered in swap call");
        }
        DataType tmp;
        for (size_t i = 0; i < numCols; ++i) {
            tmp = data[rowB];
            data[rowB] = data[rowA];
            data[rowA] = tmp;
            rowB += numRows;
            rowA += numRows;
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

    template<typename DataType, size_t numRows, size_t numCols>
    void Matrix<DataType, numRows, numCols>::subtractRow(size_t rowIndex, Vector<DataType, numCols> const& anotherRow) {
        addRow(rowIndex, static_cast<DataType>(-1)*anotherRow);
    }
}