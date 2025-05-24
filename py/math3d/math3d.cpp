#include "pybind11/pybind11.h"
#include "vector.hpp"
#include "matrix.hpp"

namespace {
    void createVectors(auto& module) {
        auto const vectorModule = module.def_submodule("vector");
        bind_Vector<double, 3>(vectorModule, "vector3");
        bind_Vector<double, 4>(vectorModule, "vector4");
        bind_Vector<double, 2>(vectorModule, "vector2");
        bind_Vector<float, 3>(vectorModule, "vector3f");
        bind_Vector<float, 4>(vectorModule, "vector4f");
        bind_Vector<float, 2>(vectorModule, "vector2f");
    }

    void createMatrices(auto& module) {
        auto const matrixModule = module.def_submodule("matrix");
        py::enum_<math3d::Order>(matrixModule, "order")
            .value("row_major", math3d::Order::RowMajor)
            .value("col_major", math3d::Order::ColumnMajor)
            .export_values();
        bind_Matrix<double, 3, 3>(matrixModule, "matrix3x3");
        bind_Matrix<double, 4, 4>(matrixModule, "matrix4x4");
    }

}

PYBIND11_MODULE(math3d, module) {
    createVectors(module);
    createMatrices(module);
}
