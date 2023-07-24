#ifndef MATH3D_MATRIX_H
#define MATH3D_MATRIX_H

#include <ostream>
#include <string>
#include <initializer_list>
#include <exception>
#include <memory>
#include <iomanip>
#include "Vector.h"

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
        Matrix(const std::initializer_list<std::initializer_list<DataType>>& initList, const Order& order = Order::RowMajor) {
            
            // allocate memory
            data.reset(new DataType[numRows * numCols]);
            
            // read and store data in data as per the format of the input data
            order == Order::ColumnMajor ? readColumnMajor(initList) : readRowMajor(initList);
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
            if(assignedColumn != -1) {
                throw std::runtime_error("Invalid assignment. Matrix::operator[] should be used to assign columns. "
                                         "An assignment must be completed before operator[] can be invoked again");
            }
            if (columnIndex >= numCols) {
                throw std::runtime_error("Invalid access. " + std::to_string(columnIndex) + " is not a valid column "
                                         "index for a matrix with " + std::to_string(numCols) + " columns");
            }
            assignedColumn = columnIndex;
            return *this;
        }

        void operator=(Vector<DataType, numCols> const& vector) {
            if(assignedColumn == -1) {
                throw std::runtime_error("Invalid assignment. Check assignment expressions");
            }
            memcpy(data.get() + assignedColumn * numCols, vector.getData(), numCols * sizeof(DataType));
            assignedColumn = -1;
        }

        Matrix transpose() {
            Matrix <DataType, numCols, numRows> transposedMatrix;
            auto& transposedData = transposedMatrix.data;
            for (auto row = 0u; row < numRows; ++row) {
                for (auto col = 0u; col < numCols; ++col) {
                    transposedData[row * numCols + col] = data[col * numRows + row];
                }
            }
            return transposedMatrix;
        }

        public:
            // Print column major matrix data in row order format
            void print(std::ostream& os) const {
                for (size_t row = 0; row < numRows; ++row) {
                    for (size_t col = 0; col < numCols; ++col) {
                        os << std::setw(10) << std::setprecision(6) << data[col * numRows + row];
                        if (col != numCols - 1) os << ' '; 
                    }
                    os << std::endl;
                }
            }

    protected:
        std::unique_ptr<DataType[]> data;
        int assignedColumn {-1};

    private:
        void assign(const Matrix& other) {
            data.reset(new DataType[numRows * numCols]);
            for(size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    auto index = col * numRows + row;
                    data[index] = other.data[index];
                }
            }
        }
        
        void readColumnMajor(const std::initializer_list<std::initializer_list<DataType>>& initList) {

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
        
        void readRowMajor(const std::initializer_list<std::initializer_list<DataType>>& initList) {

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
};

template<typename DataType, size_t numRows, size_t numCols>
std::ostream& operator<<(std::ostream& os, const Matrix<DataType, numRows, numCols>& m) {
    m.print(os);
    return os;
}

}

#endif
