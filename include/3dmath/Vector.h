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

        friend std::ostream& operator << (std::ostream& os, VectorBase const& v);
    };
    
    // A vector whose elements are stored in contiguous memory
    template<typename T, unsigned Size>
    class Vector : public VectorBase {
        static_assert(Size > 1, "Size of a vector should be greater than 1");
        // Proxy to an element in the vector
        struct Proxy;
    public:
        // Vector data accessors that allow 1,2, 3 and 4D vectors to be used in semantically meaningful fashion.
        // For example:
        //      using Point2D = Vector<float, 2>
        //      Point2D origin{10, 10};
        //      origin.x += 10
        //      origin.y += 10;
        // or
        //      using RGBColor = Vector<float,3>
        //      RGBColor color{RED, GREEN, BLUE}
        //      glClearColor(color.r, color.g, color.b);
        union {Proxy x{}; Proxy r;};
        union {Proxy y{}; Proxy g;};
        union {Proxy z{}; Proxy b;};
        union {Proxy w{}; Proxy a;};

    private:
        // Definition of element proxy. Proxy is defined private so clients of vector cannot create proxies external to a vector
        struct Proxy {
            // Update element pointed to by the proxy to T. value is const T& to accept both l and rvalues
            // e.g.
            // using Point3D = Vector<float,3>;
            // Point3D p1, p2;
            // p1.x = 100.f
            // or
            // p2.x = p1.x;
            void operator=(T const& value) { // NOLINT: We are intentionally using a different signature for = operator
                if (!data) {
                    throw std::runtime_error("Invalid access!");
                }
                *data = value;
            }
            // Convert to T so a proxy can be used in expressions that involve T
            // e.g.
            // void someFunction(float val);
            // using Point3D = Vector<float,3>;
            // Point3D p1;
            // someFunction(p1.x);
            operator T() {          // NOLINT: Supress implicit conversion clang-tidy warning since we want Proxy to be
                                    //         converted to T implicitly without client calling Proxy.operator T() or
                                    //         doing an ugly cast
                return *data;
            }
            operator T() const {    // NOLINT
                return *data;
            }

            // Convenience functions for using vector's convenience members in shorthand expressions
            void operator+=(T const& another) {
                *data += another;
            }
            void operator-=(T const& another) {
                *data -= another;
            }
            void operator*=(T const& another) {
                *data *= another;
            }
            void operator/=(T const& another) {
                *data /= another;
            }
            void operator+=(Proxy const& another) {
                *data += *(another.data);
            }
            void operator-=(Proxy const& another) {
                *data -= *(another.data);
            }
            void operator*=(Proxy const& another) {
                *data *= *(another.data);
            }
            void operator/=(Proxy const& another) {
                *data /= *(another.data);
            }

        private:
            // Clear the proxy. Permits a Vector to be moved meaningfully by allowing the r-value that's moved to have its proxies
            // null-ed out. Defined private to prevent clients of vector from clobbering the internal state of a vector
            void reset() {
                data = nullptr;
            }

            void bind(T& element) {
                if(!data) {
                    data = &element;
                } else {
                    throw std::runtime_error("Proxy already bound");
                }
            }

            T* data {};

            // Allow the outer class Vector to call reset() to facilitate move operations
            friend Vector<T, Size>;
        };

        // These macros prevent code duplication in vector's constructors and operators
        #define SET_CONVENIENCE_MEMBERS           \
            switch (Size) {                    \
            case 4:                               \
                w.bind(data[3]);                  \
            case 3:                               \
                z.bind(data[2]);                  \
            case 2:                               \
                y.bind(data[1]);                  \
            case 1:                               \
                x.bind(data[0]);                  \
            default:                              \
                break;                            \
            }

        #define RESET_CONVENIENCE_MEMBERS(Vector) \
            Vector.x.reset();                     \
            Vector.y.reset();                     \
            Vector.z.reset();                     \
            Vector.w.reset();

        public:
            Vector() {
                SET_CONVENIENCE_MEMBERS;
            }

            Vector(std::initializer_list<T> const& vals) {
                if (vals.size() != Size) {
                    throw std::invalid_argument("Dimension mismatch: Vector's dimension is " +
                                                std::to_string(Size) + " Input size is " +
                                                std::to_string(vals.size()));
                }
                for (size_t i = 0; i < vals.size(); ++i) {
                    data[i] = std::data(vals)[i];
                }
                SET_CONVENIENCE_MEMBERS;
            }

            Vector(Vector const& other) {
               this->operator=(other);
               SET_CONVENIENCE_MEMBERS;
            }

            Vector& operator=(Vector const& rhs) {
                for (unsigned i = 0; i < Size; ++i) {
                    data[i] = rhs.data[i];
                }
                return *this;
            }

            Vector(Vector&& other) noexcept {
                data = std::move(other.data);
                SET_CONVENIENCE_MEMBERS;
                RESET_CONVENIENCE_MEMBERS(other);
            }

            Vector& operator=(Vector&& other) noexcept {
                data = std::move(other.data);
                RESET_CONVENIENCE_MEMBERS(other);
                return *this;
            }

            // Conversion constructor to build from a STL vector
            explicit Vector(std::vector<T> const& v) {
                if (Size != v.size()) {
                    throw std::invalid_argument("Dimension mismatch: Vector's dimension is " +
                                                std::to_string(Size) + " Input size is " +
                                                std::to_string(v.size()));
                }
                for (size_t i = 0; i < v.size(); ++i) {
                    data[i] = v[i];
                }
                SET_CONVENIENCE_MEMBERS;
            }

            // Add this vector to another and return the sum
            Vector operator+(Vector const& another) const {
                Vector <T, Size> result;
                for (unsigned i = 0; i < Size; ++i) {
                    result.data[i] = this->data[i] + another.data[i];
                }
                return result;
            } 

            // Subtract this vector from another and return the difference
            Vector operator-(Vector const& another) const {
                Vector <T, Size> result;
                for (unsigned i = 0; i < Size; ++i) {
                    result.data[i] = this->data[i] - another.data[i];
                }
                return result;
            }

            T const& operator[](const unsigned index) const {
                if (index >= Size)
                    throw std::invalid_argument(std::to_string(index) + " is out of bounds."
                                                " Vector dimension is " + std::to_string(Size));
                return data[index];
            }

            T& operator[](const unsigned index) {
                if (index >= Size)
                    throw std::invalid_argument(std::to_string(index) + " is out of bounds."
                                                " Vector dimension is " + std::to_string(Size));
                return data[index];
            }
            
            // Compute cross product of this vector and another and return the mutually orthonormal vector
            Vector operator*(Vector const& another) const {
                static_assert(Size == 3, "Cross product can only be computed for 3D vectors");
                Vector<T, Size> result;
                auto* v1 = getData();
                auto* v2 = another.getData();
                result[0] = v1[1]*v2[2] - v1[2]*v2[1];
                result[1] = v2[0]*v1[2] - v1[0]*v2[2];
                result[2] = v1[0]*v2[1] - v1[1]*v2[0];
                return result; 
            }

            Vector operator*(T const scalar) const {
                Vector<T, Size> result;
                for(auto i = 0; i < Size; ++i) {
                    result[i] = scalar * data[i];
                }
                return result;
            }

            Vector& normalize() {
                T norm = length();
                for (size_t i = 0; i < Size; ++i) {
                    data[i] /= norm;
                }
                return *this;
            }

            Vector normalize() const {
                T norm = length();
                Vector<T, Size> result;
                for (size_t i = 0; i < Size; ++i) {
                    result.data[i] /= norm;
                }
                return result;
            }

            T length() const {
                T result = 0;
                for (size_t i = 0; i < Size; ++i) {
                    result += (data[i] * data[i]);
                }
                return static_cast<T> (sqrt(result));
            }

            void operator/=(const T scalar) {
                for (size_t i = 0; i < Size; ++i) {
                    data[i] /= scalar;
                }
            }

            void operator+=(Vector const& another) {
                for (size_t i = 0; i < Size; ++i) {
                    data[i] += another[i];
                }
            }

            float dot(Vector const& another) const {
                float proj = 0.f;
                for (size_t i = 0; i < Size; ++i)
                    proj += this->operator[](i) * another[i];
                return proj;
            }
            
            T const* getData() const { return data.data(); }
            T* getData() { return data.data(); }

        protected:
            void print(std::ostream& os) const override {
                os << '['; 
                for (auto i = 0; i < Size; ++i) {
                    os << (*this)[i];
                    os << ((i == Size - 1) ? ']' : ',');
                }
            }

        protected:
            std::array<T, Size> data{};
    };

    inline std::ostream& operator << (std::ostream& os, VectorBase const& v) {
        v.print(os);
        return os;
    }

    template <typename DataType, unsigned numRows>
    Vector<DataType, numRows> operator*(DataType scalar, Vector<DataType, numRows> const& vector) {
        return vector * scalar;
    }

    template<typename T>
    using Vector2D = Vector<T,2>;

    template<typename T>
    using Vector3D = Vector<T,3>;

    template<typename T>
    using Vector4D = Vector<T,4>;

}