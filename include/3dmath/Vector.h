#ifndef MATH3D_MATRIX_H
#define MATH3D_MATRIX_H

#include <ostream>
#include <string>
#include <initializer_list>
#include <exception>
#include <memory>
#include <cmath>

namespace math3d {
    
    // A do-nothing non-template base class to enable printing a vector via
    // stream insertion overload operator If the stream operator is defined as
    // a friend of the template derived type, because of the fact that the
    // friend function definition is not tied to the template instantiation,
    // the friend function remains undefined leading to linker errors. Making
    // the friend function take a non-template parameter avoids this issue 
    class VectorBase {
        protected:
            virtual void print(std::ostream& os) const = 0;

        friend std::ostream& operator << (std::ostream& os, const VectorBase& v);
    };
    
    // A vector in column major format 
    template<typename DataType, unsigned numRows>
    class Vector : public VectorBase {
        
        public:
            Vector() {
                m_data = std::make_unique<DataType[]>(numRows);
                for (unsigned i = 0; i < numRows; ++i) {
                    m_data.get()[i] = 0.f;
                }
            }

            Vector(const std::initializer_list<DataType>& vals) {
                if (vals.size() != numRows) {
                    throw std::invalid_argument("Dimension mismatch: Vector's dimension is " + 
                                                 std::to_string(numRows) + " Input size is " +
                                                 std::to_string(vals.size()));
                }
                m_data = std::make_unique<DataType[]>(numRows);
                for (size_t i = 0; i < vals.size(); ++i) {
                    m_data.get()[i] = data(vals)[i];
                }
            }

            Vector(const Vector& other) {
               this->operator=(other); 
            } 

            Vector(Vector&& rval) {
                // NOTE: Unlike copy constructor, we are unable to invoke move assignment
                // from the move constructor. The rval's data pointer is not set to null
                // Is r-value using pass by copy semantics?
                m_data = std::move(rval.m_data);
                rval.m_data = nullptr;
            }
            
            // Conversion constructor to build from a STL vector
            // TODO: Support building from other container types 
            Vector(const std::vector<DataType>& v) {
                if (numRows != v.size()) { 
                    throw std::invalid_argument("Dimension mismatch: Vector's dimension is " + 
                                                 std::to_string(numRows) + " Input size is " +
                                                 std::to_string(v.size()));
                }
                m_data = std::make_unique<DataType[]>(numRows);
                for (size_t i = 0; i < v.size(); ++i) {
                    m_data.get()[i] = v[i];
                }
            }

            Vector& operator=(const Vector& rhs) { 
                m_data.reset(new DataType[numRows]);
                for (unsigned i = 0; i < numRows; ++i) {
                    m_data.get()[i] = rhs.m_data.get()[i];
                } 
                return *this; 
            }
            
            Vector& operator=(Vector&& rval) { 
                m_data = std::move(rval.m_data);
                rval.m_data = nullptr;
                return *this;
            }
            
            // Add this vector to another and return the sum
            Vector operator+(const Vector& another) const {
                Vector <DataType, numRows> result;
                for (unsigned i = 0; i < numRows; ++i) {
                    result.m_data.get()[i] = this->m_data.get()[i] + another.m_data.get()[i];
                }
                return result;
            } 

            // Subtract this vector from another and return the difference
            Vector operator-(const Vector& another) const {
                const float* lhs = getData();
                const float* rhs = another.getData(); 
                return Vector<float,3>({lhs[0]-rhs[0], lhs[1]-rhs[1], lhs[2]-rhs[2]});
            }

            float& operator[](const unsigned index) const {
                if (index >= numRows) 
                    throw std::invalid_argument(std::to_string(index) + " is out of bounds."
                                                " Vector dimension is " + std::to_string(numRows));
                return m_data.get()[index];
            }
            
            // Compute cross product of this vector and another and return the mutally orthonormal vector 
            Vector operator*(const Vector& another) const {
                // TODO: Investigate possibility of static_assert
                if (numRows != 3) {
                    throw std::runtime_error("Cross product can only be computed for 3D vectors");
                }
                Vector<DataType, numRows> result;
                const float* v1 = getData();
                const float* v2 = another.getData();
                result[0] = v1[1]*v2[2] - v1[2]*v2[1];
                result[1] = v2[0]*v1[2] - v1[0]*v2[2];
                result[2] = v1[0]*v2[1] - v1[1]*v2[0];
                return result; 
            }

            Vector& normalize() {
                float norm = length();
                float* v = const_cast<float*>(getData());
                for (size_t i = 0; i < numRows; ++i) {
                    v[i] /= norm;
                }
                return *this;
            }

            float length() {
                const float* v = getData();
                float result = 0;
                for (size_t i = 0; i < numRows; ++i) {
                    result += (v[i] * v[i]);
                }
                return sqrt(result);
            }

            void operator/=(const DataType scalar) {
                float* v = m_data.get();
                for (size_t i = 0; i < numRows; ++i) {
                    v[i] /= scalar;
                }
            }

            void operator+=(const Vector& another) {
                float* v = m_data.get();
                for (size_t i = 0; i < numRows; ++i) {
                    v[i] += another[i];
                }
            }

            float dot(const Vector& another) {
                float proj = 0.f;
                for (size_t i = 0; i < numRows; ++i)
                    proj += this->operator[](i) * another[i];
                return proj;
            }
            
            DataType const* getData() const { return m_data.get(); }

        protected:
            void print(std::ostream& os) const override {
                os << '['; 
                for (auto i = 0; i < numRows; ++i) {
                    os << (*this)[i];
                    os << ((i == numRows-1) ? ']' : ',');
                }
            }

        private:
            std::unique_ptr<DataType[]> m_data; 

    }; 

    inline std::ostream& operator << (std::ostream& os, const VectorBase& v) {
        v.print(os);
        return os;
    }
    
    // TODO: Define convenience class managing 3D vectors 
    /*template <typename T>
    class 3DVector : public Vector<T, 3> {
        public:
            T& x;
            T& y;
            T& z;
            3DVector() 
                : x(m_data.get()[0])
                , y(m_data.get()[1])
                , z(m_data.get()[2]) {
            }
    };*/
}



#endif
