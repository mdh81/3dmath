#pragma once
#include <ostream>
#include <string>
#include <initializer_list>
#include <exception>
#include <memory>
#include <sstream>
#include <iomanip>
#include "Vector.h"
#include "Constants.h"

namespace math3d {

// A column-major matrix that stores its elements in contiguous memory

// By default, the implementation assumes that the input data passed to it
// is in row major order. Allowing the client to use row-major order allows
// the client to lay out columns of their column-major matrices as columns.
// For example,
// auto myMatrix = Matrix<float, 4, 4> {
// {a, e, i, m},
// {b, f, j, n},
// {c, g, k, o},
// {d, h, l, p}};
// If the code assumed column-major then this matrix in code would be
// transposed and could cause misinterpretation.
//
// The order of the input can be controlled by using the constructor
// argument Order.
enum class Order {
    ColumnMajor,
    RowMajor
};

// TODO: Convert size_t to unsigned to match Vector

template<typename DataType, size_t numRows, size_t numCols>
class Matrix {
    
    public:

        // Default construction
        Matrix() : data(new DataType[numRows * numCols]) {
            for(size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    data[row * numCols + col] = DataType{0};
                }
            }
        }
        
        // Construction via an initializer list of initializer lists 
        // Each sub-initializer defines a column or row of matrix data.
        // The enum Order argument specifies the format of the data. 
        // If the order is column major, then each sub-initializer is
        // treated as a column of data otherwise the data is assumed
        // to be in the row major order
        Matrix(std::initializer_list<std::initializer_list<DataType>> const& initList, Order const& order = Order::RowMajor) {
            
            // allocate memory
            data.reset(new DataType[numRows * numCols]);
            
            // read and store data in data as per the format of the input data
            order == Order::ColumnMajor ? readColumnMajor(initList) : readRowMajor(initList);
        }

        // Construct with data from a 1D vector. This is useful to build minors and cofactors
        Matrix(std::vector<DataType> const& inputData, Order const& order = Order::RowMajor) {
            // allocate memory
            data.reset(new DataType[numRows * numCols]);
            for (int i = 0; i < numRows; ++i) {
                for (int j = 0; j < numCols; ++j) {
                    data[i * numRows + j] =
                            order == Order::ColumnMajor ?
                            inputData[i * numRows + j] : inputData[j * numCols + i];
                }
            }
        }
        
        // Copy construction 
        Matrix(const Matrix& other) {
            assign(other);
        }
        
        // Copy assignment
        Matrix& operator=(const Matrix& other) {
            assign(other);
            return *this;
        }

        // Move construction
        Matrix(Matrix&& other)  noexcept {
            data = std::move(other.data);
        }
        
        // Move assignment
        Matrix& operator=(Matrix&& other)  noexcept {
            data = std::move(other.data);
        }
        
        // Destructor 
        ~Matrix()  = default;

        operator DataType const*() { // NOLINT: Implicit conversion is the point of defining this operator
            return data.get();
        }

        // Vector multiplication
        auto operator*(Vector<DataType, numRows> const& inputVector) const {
            Vector<DataType, numRows> outputVector;
            for (auto row = 0u; row < numRows; ++row) {
                for (auto col = 0u; col < numCols; ++col) {
                    outputVector[row] += data[col * numRows + row] * inputVector[col];
                }
            }
            return outputVector;
        }

        [[nodiscard]]
        size_t getNumberOfRows() const {
            return numRows;
        }

        [[nodiscard]]
        size_t getNumberOfColumns() const {
            return numCols;
        }

        [[nodiscard]]
        const DataType* getData() const {
            return data.get();
        }

        [[nodiscard]]
        Vector<float, numCols> getColumn(unsigned columnIndex) const {
            Vector<float, numCols> columnVector;
            for (size_t row = 0; row < numRows; ++row) {
                columnVector[row] = data[columnIndex * numRows + row];
            }
            return columnVector;
        }

        [[nodiscard]]
        Vector<float, numCols> getRow(unsigned rowIndex) const {
            Vector<float, numRows> rowVector;
            auto offset = rowIndex;
            for (size_t col = 0; col < numCols; ++col) {
                rowVector[col] = data[offset];
                offset += numCols;
            }
            return rowVector;
        }

        // These two overloads allow expressions of the form matrix[0] = vector
        Matrix& operator[](unsigned columnIndex) {
            if (columnIndex >= numCols) {
                throw std::runtime_error("Invalid access. " + std::to_string(columnIndex) + " is not a valid column "
                                         "index for a matrix with " + std::to_string(numCols) + " columns");
            }
            currentColumn = columnIndex;
            return *this;
        }

        void operator=(Vector<DataType, numCols> const& vector) {
            if(currentColumn == -1) {
                throw std::runtime_error("Invalid assignment. Check assignment expressions");
            }
            memcpy(data.get() + currentColumn * numCols, vector.getData(), numCols * sizeof(DataType));
        }

        // Column access
        // Allows expression of the form Vector<DataType, numRows> column0 = Matrix[0]
        operator Vector<DataType, numRows>() const {
            if(currentColumn == -1) {
                throw std::runtime_error("Invalid conversion to vector. Check assignment expressions");
            }
            Vector<DataType, numRows> result;
            for (size_t i = 0, index = currentColumn * numRows; i < numRows; ++i) {
                result[i] = data[index+i];
            }
            return result;
        }

        // Row access
        Vector<DataType, numCols> operator()(size_t rowIndex) const {
            Vector<DataType, numCols> result;
            for (size_t i = 0, index = rowIndex; i < numCols; ++i, index += numRows) {
                result[i] = data[index];
            }
            return result;
        }

        // TODO: Check these with non-square matrices. They look suspect
        // Element access operators to allow assignment of individual elements in the form of expression
        // matrix(a, b) = c
        Matrix& operator()(size_t rowIndex, size_t columnIndex) {
            validateElementAccess(rowIndex, columnIndex);
            currentColumn = columnIndex;
            assignedRow = rowIndex;
            return *this;
        }

        void operator=(DataType value) {
            if (currentColumn != -1 && assignedRow != -1) {
                data[currentColumn * numCols + assignedRow] = value;
                currentColumn = -1;
                assignedRow = -1;
            } else {
                throw std::runtime_error("Invalid assignment. Check assignment expressions");
            }
        }

        // Conversion operator that allows extraction of the current element
        // This allows "matrix(a, b)" to appear in non-assignment contexts
        // float xyz = matrix(a, b);
        operator DataType() const {
            DataType scalar;
            if (currentColumn != -1 && assignedRow != -1) {
                scalar = data[currentColumn * numCols + assignedRow];
                currentColumn = -1;
                assignedRow = -1;
            } else {
                throw std::runtime_error("Invalid conversion. Check element access expressions");
            }
            return scalar;
        }

        // Element access for const objects
        DataType operator()(size_t rowIndex, size_t columnIndex) const {
            validateElementAccess(rowIndex, columnIndex);
            return data[rowIndex * numCols + columnIndex];
        }

        // Print column major matrix data in row order format
        void print(std::ostream& os) const {
            for (size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    auto val = data[col * numRows + row];
                    if (fabs(val) < math3d::constants::tolerance) {
                        val = 0;
                    }
                    os << std::setw(10) << std::setprecision(6) << val;
                    if (col != numCols - 1) os << ' ';
                }
                os << std::endl;
            }
        }

        // Defined in MatrixOperations.h
        Matrix transpose();
        DataType determinant();
        Matrix inverse();
    private:
        void swapRows(size_t rowA, size_t rowB);
        void addRow(size_t rowIndex, Vector<DataType, numCols> const& anotherRow);

    protected:
        std::unique_ptr<DataType[]> data;
        mutable int currentColumn {-1};
        mutable int assignedRow {-1};

    private:
        Matrix<DataType, numRows-1, numCols-1> getMinor(unsigned row, unsigned column) {
            static_assert(numRows >= 2 && numCols >=2, "1x1 matrices don't have a minor");
            std::vector<DataType> newMatrixData;
            newMatrixData.reserve((numRows - 1) * (numCols - 1));
            for (size_t rowIndex = 0; rowIndex < numRows; ++rowIndex) {
                for (size_t colIndex = 0; colIndex < numCols; ++colIndex) {
                    if (rowIndex != row && colIndex != column) {
                        newMatrixData.push_back(data[colIndex * numCols + rowIndex]);
                    }
                }
            }
            return Matrix<DataType, numRows-1, numCols-1>(newMatrixData);
        }

        Matrix<DataType, numRows-1, numCols-1> getCofactor(unsigned row, unsigned column) {
            static_assert(numRows >=3 && numCols >=2, "Minor can only be calculated for 3x3 matrices");
            return pow(-1, row+column) * getMinor(row, column);
        }

        Matrix adjoint() {
            return {};
        }

        void validateElementAccess(size_t rowIndex, size_t columnIndex) const {
            auto badRowIndex = rowIndex >= numRows;
            auto badColumnIndex = columnIndex >= numCols;
            if (badRowIndex || badColumnIndex) {
                std::stringstream errorMessage;
                errorMessage << "Invalid access: ";
                if (badRowIndex) {
                    errorMessage << rowIndex << " is not a valid row index";
                    errorMessage << " for a " << numRows << 'x' << numCols << " matrix" << std::endl;
                }
                if (badColumnIndex) {
                    errorMessage << columnIndex << " is not a valid column index";
                    errorMessage << " for a " << numRows << 'x' << numCols << " matrix" << std::endl;
                }
                throw std::runtime_error(errorMessage.str());
            }
        }

        void assign(Matrix const& other) {
            data.reset(new DataType[numRows * numCols]);
            for(size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    auto index = col * numRows + row;
                    data[index] = other.data[index];
                }
            }
        }
        
        void readColumnMajor(std::initializer_list<std::initializer_list<DataType>> const& initList) {

            // Number of columns in input data should match numCols 
            if (numCols != initList.size()) {
                throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Number of columns in the input is " + std::to_string(initList.size())));
            }
            
            // Number of rows for each column of the input data should match numRows 
            for (size_t col = 0; col < numCols; ++col) {
                if (std::data(initList)[col].size() != numRows) {
                    throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Number of rows in column " + std::to_string(col+1) + 
                                    " is " + std::to_string(std::data(initList)[col].size())));
                }
            }
            
            // Read and store data in column major format 
            for (size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    data[col * numRows + row] = std::data(std::data(initList)[col])[row];
                }
            }
        }
        
        void readRowMajor(std::initializer_list<std::initializer_list<DataType>> const& initList) {

            // Number of rows in input data should match numRows 
            if (numRows != initList.size()) {
                throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Number of rows in the input is " + std::to_string(initList.size())));
            }
            
            // Number of columns in each row of the input data should match numCols 
            for (size_t row = 0; row < numRows; ++row) {
                if (std::data(initList)[row].size() != numCols) {
                    throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Number of columns in row " + std::to_string(row+1) + 
                                    " is " + std::to_string(std::data(initList)[row].size())));
                }
            }

            // Read row major data and store in column major format 
            for (size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    data[col * numRows + row] = std::data(std::data(initList)[row])[col];
                }
            }
        }

        template<size_t, size_t>
        friend class MatrixTestWrapper;
};

template<typename DataType, size_t numRows, size_t numCols>
std::ostream& operator<<(std::ostream& os, const Matrix<DataType, numRows, numCols>& m) {
    m.print(os);
    return os;
}

}
