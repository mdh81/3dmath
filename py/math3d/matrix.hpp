#pragma once
#include "pybind11/pybind11.h"
#include "pybind11/operators.h"

#include "MatrixOperations.h"

namespace py = pybind11;

template<typename T, uint32_t Rows, uint32_t Cols>
void bind_Matrix(py::module_ const& module, char const* className) {

    using matrix_n_n = math3d::Matrix<T, Rows, Cols>;
    using vector_n = math3d::Vector<T, Cols>;

    py::class_<matrix_n_n>(module, className)
    .def(py::init())
    .def(py::init([](py::iterable const& data, math3d::Order const order) {
        std::vector<std::vector<T>> dataAsVec;
        for (auto outer : data) {
            std::vector<T> inner = outer.cast<std::vector<T>>();
            dataAsVec.push_back(std::move(inner));
        }
        return matrix_n_n{dataAsVec, order};
    }))
    .def("__getitem__", [](matrix_n_n const& matrix, uint32_t const index) {
        return matrix.operator[](index);
    })
    .def("__getitem__", [](matrix_n_n const& matrix, std::pair<uint32_t, uint32_t> const& index_pair) {
        return matrix.operator()(index_pair.first, index_pair.second);
    })
    .def("row", [](matrix_n_n const& matrix, uint32_t row) {
        return matrix.operator()(row);
    })
    .def(py::self * py::self)
    .def(py::self * vector_n{})
    .def("__str__", &matrix_n_n::asString)
    .def("__repr__", &matrix_n_n::asString)

    // Operations
    .def("transpose", &matrix_n_n::transpose)
    .def("ur", [](matrix_n_n const& input) {
        matrix_n_n output;
        input.convertToUpperTriangular(output);
        return output;
    })
    ;
}