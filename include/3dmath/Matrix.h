#ifndef MATH3D_MATRIX_H
#define MATH3D_MATRIX_H

#include <ostream>
#include <string>
#include <initializer_list>
#include <exception>

namespace math3d {

// A column-major matrix that stores its elements in contiguous memory

template<typename DataType, size_t numRows, size_t numCols>
class Matrix {
    public:

        // Default construction
        Matrix() {
            m_data = new DataType[numRows][numCols]; 
            for(size_t i = 0; i < numRows; ++i) {
                for (size_t j = 0; j < numCols; ++j) {
                    m_data[i][j] = DataType{0};
                }
            }

        }
        
        // Construction via an initializer 
        template<typename ListType>
        Matrix(const std::initializer_list<std::initializer_list<ListType>>& initList) {
            
            static_assert(std::is_same<ListType, DataType>::value, 
                          "Matrix and its initializer should be of the same type");
            
            // prevents a crash during destruction in case an exception is thrown and m_data is deleted during stack unrolling 
            m_data = nullptr;
            
            if (numCols != initList.size()) {
                throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "Outer initializer list's size is " + std::to_string(initList.size())));
            }
            
            for (size_t i = 0; i < numCols; ++i) {
                if (data(initList)[i].size() != numRows) {
                    throw std::invalid_argument(
                        std::string("Incompatible dimensions: Matrix dimensions are [" + 
                                    std::to_string(numRows) + ',' + std::to_string(numCols) + "] " +
                                    "One of the inner initializer list's size is " + std::to_string(initList.size())));
                }
            }
            
            m_data = new DataType[numRows][numCols];
            for (size_t i = 0; i < numCols; ++i) {
                for (size_t j = 0; j < numRows; ++j) {
                    m_data[i][j] = data(data(initList)[i])[j]; 
                } 
            }
        }
        
        // Copy construction 
        Matrix(const Matrix& other) {
            m_data = nullptr;
            assign(other);
        }
        
        // Copy assignment
        Matrix& operator=(const Matrix& other) {
            assign(other);
            return *this;
        }

        // Move construction
        Matrix(Matrix&& other) {
            m_data = std::move(other.m_data);
            other.m_data = nullptr;
        }
        
        // Move assignment
        Matrix& operator=(Matrix&& other) {
            m_data = std::move(other.m_data);
            other.m_data = nullptr;
        }
        
        // Destructor 
        ~Matrix() {
            delete[] m_data;
        }

        size_t getNumberOfRows() const { return numRows; }
        size_t getNumberOfColumns() const { return numCols; }
        const DataType* getData()  const { return m_data[0]; }
        
        private: 
            void print(std::ostream& os) const {
                for (size_t i = 0; i < numRows; ++i) {
                    for (size_t j = 0; j < numCols; ++j) {
                        os << m_data[j][i] << ' ';
                    }
                    os << std::endl;
                }
            }

    protected:
        // Pointer to an array of columns
        // This format stores elements of this matrix in contiguous memory
        // enabling conversion of this type to a pointer and access the stored 
        // data as a 1D-array 
        DataType (*m_data)[numCols];

    private:
        void assign(const Matrix& other) {
            delete [] m_data;
            m_data = new DataType[numRows][numCols];
            for(size_t i = 0; i < numRows; ++i) {
                for (size_t j = 0; j < numCols; ++j) {
                    m_data[i][j] = other.m_data[i][j];
                }
            }
        }

};

template<typename DataType, size_t numRows, size_t numCols> requires (numRows == numCols)
class IdentityMatrix : public Matrix<DataType,numRows,numCols> {
    private:
        using Matrix<DataType,numRows,numCols>::m_data;
    public:
        IdentityMatrix() {
            for(size_t i = 0; i < numCols; ++i) {
                for (size_t j = 0; j < numRows; ++j) {
                    if (i == j) {
                        m_data[i][j] = DataType{1};
                    }   
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
