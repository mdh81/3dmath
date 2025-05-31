#include "pybind11/pybind11.h"
#include "vector.hpp"
#include "matrix.hpp"
#include "linear_system.hpp"

#include <unordered_map>

namespace {

#ifdef MAX_DIMENSIONS
    auto constexpr MaxDimension {MAX_DIMENSIONS};
#else
    auto constexpr MaxDimension {4};
#endif

    enum class Type : uint8_t {
        Vector,
        Matrix,
        LinearSystem
    };
    std::unordered_map<Type, char const*> TypePrefixMap {
        {Type::Vector, "vector"},
        {Type::Matrix, "matrix"},
        {Type::LinearSystem, "linear_system"}
    };

    template<typename Type, Type Start, Type End>
    class TypeIterator {
    public:
        TypeIterator() = default;

        TypeIterator begin() {
            return *this;
        }

        TypeIterator end() {
            return TypeIterator{End + 1};
        }

        bool operator==(TypeIterator const& another) {
            return value == another.value;
        }

    private:
        std::underlying_type_t<Type> value {Start};
    };

    auto pyTypeCreator = [](Type const type, auto& pythonModule, auto integerConstant) {
        constexpr unsigned dimension = integerConstant + 2;
        auto const typeName = TypePrefixMap[type] + std::to_string(dimension);
        if (typeName.empty()) {
            throw std::runtime_error{std::format("Unknown type {}", std::to_underlying(type))};
        }
        switch (type) {
            case Type::Vector:
                bind_Vector<double, dimension>(pythonModule, typeName.c_str());
                break;
            case Type::Matrix:
                bind_Matrix<double, dimension, dimension>(pythonModule, typeName.c_str());
                break;
            case Type::LinearSystem:
                bind_linearSystem<double, dimension>(pythonModule, typeName.c_str());
            default:
                break;
        }
    };

    template <typename ModuleType, unsigned... integers>
    constexpr void createPythonTypes(Type const type, ModuleType& module, std::integer_sequence<unsigned, integers...>) {
        (pyTypeCreator(type, module, std::integral_constant<unsigned, integers>{}), ...);
    }
}

PYBIND11_MODULE(math3d, module) {
    // NOTE: std::make_integer_sequence returns integer_sequence<unsigned, 0, 1, ... , N-1>
    // Therefore, when MaxDimension is 4, the integer sequence is 0, 1, 2 (range [0, 3)), which gets is translated to
    // types with suffix 2, 3, 4 e.g. vec2, vec3, and vec4
    auto constexpr intSeq  = std::make_integer_sequence<unsigned, MaxDimension-1>{};
    createPythonTypes(Type::Vector, module, intSeq);
    py::enum_<math3d::Order>(module, "order")
        .value("row_major", math3d::Order::RowMajor)
        .value("col_major", math3d::Order::ColumnMajor)
        .export_values();
    createPythonTypes(Type::Matrix, module, intSeq);
    createPythonTypes(Type::LinearSystem, module, intSeq);
}
