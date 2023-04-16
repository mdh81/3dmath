#ifndef MATH3D_MATRIX_H
#define MATH3D_MATRIX_H

#include <ostream>
#include <string>
#include <initializer_list>
#include <exception>
#include <memory>
#include "Vector.h"

namespace math3d {

// A column-major matrix that stores its elements in contiguous memory

template<typename DataType, size_t numRows, size_t numCols>
class Matrix {
    
    public:
        enum class Order {
            ColumnMajor,
            RowMajor
        };

        // Default construction
        Matrix() : m_data(new DataType[numRows * numCols]) {
            for(size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    m_data[row * numCols + col] = DataType{0};
                }
            }
        }
        
        // Construction via an initializer list of initializer lists 
        // Each sub-initializer defines a column or row of matrix data.
        // The enum Order argument specifies the format of the data. 
        // If the order is column major, then each sub-initializer is
        // treated as a column of data otherwise the data is assumed
        // to be in the row major order
        Matrix(const std::initializer_list<std::initializer_list<DataType>>& initList, const Order& order = Order::ColumnMajor) {
            
            // allocate memory
            m_data.reset(new DataType[numRows*numCols]);
            
            // read and store data in m_data as per the format of the input data 
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
            m_data = std::move(other.m_data);
        }
        
        // Move assignment
        Matrix& operator=(Matrix&& other)  noexcept {
            m_data = std::move(other.m_data);
        }
        
        // Destructor 
        ~Matrix()  = default;

        operator DataType const*() { // NOLINT: Implicit conversion is the point of defining this operator
            return m_data.get();
        }

        // Vector multiplication
        auto operator*(Vector<DataType, numRows> const& inputVector) {
            Vector<DataType, numRows> outputVector;
            for (auto row = 0u; row < numRows; ++row) {
                for (auto col = 0u; col < numCols; ++col) {
                    outputVector[row] += m_data[col * numRows + row] * inputVector[col];
                }
            }
            return outputVector;
        }

        [[nodiscard]] size_t getNumberOfRows() const { return numRows; }
        [[nodiscard]] size_t getNumberOfColumns() const { return numCols; }
        const DataType* getData()  const { return m_data.get(); }
        
        public:
            // Print column major matrix data in row order format
            void print(std::ostream& os) const {
                for (size_t row = 0; row < numRows; ++row) {
                    for (size_t col = 0; col < numCols; ++col) {
                        os << std::setw(10) << std::setprecision(6) << m_data[col * numRows + row];
                        if (col != numCols - 1) os << ' '; 
                    }
                    os << std::endl;
                }
            }

    protected:
        std::unique_ptr<DataType[]> m_data;

    private:
        void assign(const Matrix& other) {
            m_data.reset(new DataType[numRows * numCols]);
            for(size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    auto index = col * numRows + row;
                    m_data[index] = other.m_data[index];
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
                if (data(initList)[col].size() != numRows) {
                    throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Number of rows in column " + std::to_string(col+1) + 
                                    " is " + std::to_string(data(initList)[col].size())));
                }
            }
            
            // Read and store data in column major format 
            for (size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    m_data[col * numRows + row] = data(data(initList)[col])[row];
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
                if (data(initList)[row].size() != numCols) {
                    throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Number of columns in row " + std::to_string(row+1) + 
                                    " is " + std::to_string(data(initList)[row].size())));
                }
            }

            // Read row major data and store in column major format 
            for (size_t col = 0; col < numCols; ++col) {
                for (size_t row = 0; row < numRows; ++row) {
                    m_data[col * numRows + row] = data(data(initList)[row])[col];
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
