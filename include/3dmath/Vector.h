#pragma once
#include <ostream>
#include <string>
#include <initializer_list>
#include <exception>
#include <memory>
#include <cmath>
#include <array>

namespace math3d {

    // A do-nothing non-template base class to enable printing a vector via
    // stream insertion overload operator If the stream operator is defined as
    // a friend of the template derived type, because the
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
                for (unsigned i = 0; i < numRows; ++i) {
                    m_data[i] = 0.f;
                }
            }

            Vector(const std::initializer_list<DataType>& vals) {
                if (vals.size() != numRows) {
                    throw std::invalid_argument("Dimension mismatch: Vector's dimension is " + 
                                                 std::to_string(numRows) + " Input size is " +
                                                 std::to_string(vals.size()));
                }
                for (size_t i = 0; i < vals.size(); ++i) {
                    m_data[i] = data(vals)[i];
                }
            }

            Vector(const Vector& other) {
               this->operator=(other); 
            } 

            // Conversion constructor to build from a STL vector
            // TODO: Support building from other container types 
            Vector(const std::vector<DataType>& v) {
                if (numRows != v.size()) { 
                    throw std::invalid_argument("Dimension mismatch: Vector's dimension is " + 
                                                 std::to_string(numRows) + " Input size is " +
                                                 std::to_string(v.size()));
                }
                for (size_t i = 0; i < v.size(); ++i) {
                    m_data[i] = v[i];
                }
            }

            Vector& operator=(const Vector& rhs) { 
                for (unsigned i = 0; i < numRows; ++i) {
                    m_data[i] = rhs.m_data[i];
                } 
                return *this; 
            }
            
            // Add this vector to another and return the sum
            Vector operator+(const Vector& another) const {
                Vector <DataType, numRows> result;
                for (unsigned i = 0; i < numRows; ++i) {
                    result.m_data[i] = this->m_data[i] + another.m_data[i];
                }
                return result;
            } 

            // Subtract this vector from another and return the difference
            Vector operator-(const Vector& another) const {
                return Vector<float,3>({m_data[0]-another.m_data[0], m_data[1]-another.m_data[1], m_data[2]-another.m_data[2]});
            }

            DataType const& operator[](const unsigned index) const {
                if (index >= numRows) 
                    throw std::invalid_argument(std::to_string(index) + " is out of bounds."
                                                " Vector dimension is " + std::to_string(numRows));
                return m_data[index];
            }

            DataType& operator[](const unsigned index) {
                if (index >= numRows)
                    throw std::invalid_argument(std::to_string(index) + " is out of bounds."
                                                " Vector dimension is " + std::to_string(numRows));
                return m_data[index];
            }
            
            // Compute cross product of this vector and another and return the mutually orthonormal vector
            Vector operator*(const Vector& another) const {
                static_assert(numRows == 3, "Cross product can only be computed for 3D vectors");
                Vector<DataType, numRows> result;
                auto* v1 = getData();
                auto* v2 = another.getData();
                result[0] = v1[1]*v2[2] - v1[2]*v2[1];
                result[1] = v2[0]*v1[2] - v1[0]*v2[2];
                result[2] = v1[0]*v2[1] - v1[1]*v2[0];
                return result; 
            }

            Vector& normalize() {
                float norm = length();
                for (size_t i = 0; i < numRows; ++i) {
                    m_data[i] /= norm;
                }
                return *this;
            }

            float length() {
                float result = 0;
                for (size_t i = 0; i < numRows; ++i) {
                    result += (m_data[i] * m_data[i]);
                }
                return sqrt(result);
            }

            void operator/=(const DataType scalar) {
                for (size_t i = 0; i < numRows; ++i) {
                    m_data[i] /= scalar;
                }
            }

            void operator+=(const Vector& another) {
                for (size_t i = 0; i < numRows; ++i) {
                    m_data[i] += another[i];
                }
            }

            float dot(const Vector& another) {
                float proj = 0.f;
                for (size_t i = 0; i < numRows; ++i)
                    proj += this->operator[](i) * another[i];
                return proj;
            }
            
            DataType const* getData() const { return m_data.data(); }
            DataType* getData() { return m_data.data(); }

        protected:
            void print(std::ostream& os) const override {
                os << '['; 
                for (auto i = 0; i < numRows; ++i) {
                    os << (*this)[i];
                    os << ((i == numRows-1) ? ']' : ',');
                }
            }

        protected:
            std::array<DataType, numRows> m_data;

    }; 

    inline std::ostream& operator << (std::ostream& os, const VectorBase& v) {
        v.print(os);
        return os;
    }

    template<typename T>
    struct Vector2D : public Vector<T, 2> {

        T& x;
        T& y;

        // Bind x and y to element 0 and 1 of the raw array
        Vector2D()
        : Vector<T,2>()
        , x(Vector<T,2>::m_data[0])
        , y(Vector<T,2>::m_data[1]) {

        }

        // Emplace support constructor
        Vector2D(T const x, T const y)
        : Vector2D() {
            this->x = x;
            this->y = y;
        }

        // Define explicitly to get around implicit deletion due to reference member
        Vector2D(std::initializer_list<T> const& list)
                : Vector<T,2> (list)
                , x(Vector<T,2>::m_data[0])
                , y(Vector<T,2>::m_data[1]) {

        }

        // Define explicitly to get around implicit deletion due to reference member
        Vector2D& operator=(Vector2D const& another) {
            Vector<T,2>::operator=(another);
            return *this;
        }

    };

    template <typename T>
    struct Vector3D : public Vector<T, 3> {
        T& x;
        T& y;
        T& z;
        Vector3D()
            : x(Vector<T,3>::m_data[0])
            , y(Vector<T,3>::m_data[1])
            , z(Vector<T,3>::m_data[2]) {
        }

        // Emplace support constructor
        Vector3D(T const x, T const y, T const z)
        : Vector3D() {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        // Define explicitly to get around implicit deletion due to reference member
        Vector3D(const std::initializer_list<T>& list)
        : Vector<T,3> (list)
        , x(Vector<T,3>::m_data[0])
        , y(Vector<T,3>::m_data[1])
        , z(Vector<T,3>::m_data[2]) {

        }

        // Define explicitly to get around implicit deletion due to reference member
        Vector3D& operator=(const Vector3D& another) {
            Vector<T,3>::operator=(another);
            x = Vector<T,3>::m_data[0];
            y = Vector<T,3>::m_data[1];
            z = Vector<T,3>::m_data[2];
            return *this;
        }

        // Conversion constructor to build a Vector3D from Vector<T,3>
        // Allows the following expression:
        // Vector3D c = a + b;
        Vector3D(Vector<T,3> const& another)
        : Vector<T,3>(another)
        , x(Vector<T, 3>::m_data[0])
        , y(Vector<T, 3>::m_data[1])
        , z(Vector<T, 3>::m_data[2]) {

        }
    };

    template <typename T>
    struct Vector4D : public Vector<T, 4> {
        T& x;
        T& y;
        T& z;
        T& w;
        Vector4D()
        : x(Vector<T,4>::m_data[0])
        , y(Vector<T,4>::m_data[1])
        , z(Vector<T,4>::m_data[2])
        , w(Vector<T,4>::m_data[3]) {
        }

        // Emplace support constructor
        Vector4D(T const x, T const y, T const z)
        : Vector4D() {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        // Define explicitly to get around implicit deletion due to reference member
        Vector4D(const std::initializer_list<T>& list)
        : Vector<T,4> (list)
        , x(Vector<T,4>::m_data[0])
        , y(Vector<T,4>::m_data[1])
        , z(Vector<T,4>::m_data[2])
        , w(Vector<T,4>::m_data[3]) {
        }

        // Define explicitly to get around implicit deletion due to reference member
        Vector4D& operator=(const Vector4D& another) {
            Vector<T,4>::operator=(another);
            x = Vector<T,4>::m_data[0];
            y = Vector<T,4>::m_data[1];
            z = Vector<T,4>::m_data[2];
            w = Vector<T,4>::m_data[3];
            return *this;
        }

        // Conversion constructor to build a Vector4D from Vector<T,4>
        // Allows the following expression:
        // Vector4D c = a + b;
        Vector4D(Vector<T,4> const& another)
        : Vector<T,4>(another)
        , x(Vector<T,4>::m_data[0])
        , y(Vector<T,4>::m_data[1])
        , z(Vector<T,4>::m_data[2])
        , w(Vector<T,4>::m_data[3]) {

        }
    };

}