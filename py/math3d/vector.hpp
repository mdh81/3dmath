#pragma once
#include "pybind11/pybind11.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"
#include "util.h"
#include "Vector.h"

namespace py = pybind11;

template<typename T, uint32_t Size>
void bind_Vector(py::module_ const& module, char const* className) {
    using vector_n = math3d::Vector<T, Size>;
    auto vec_class =
        py::class_<vector_n>(module, className)
        // Construction
        .def(py::init())
        .def(py::init([](py::list const& list) {
            auto const input = list.cast<std::vector<T>>();
            return vector_n{input};
        }))
        // Member access
        .def_property("x",
            [](vector_n const& self) {
                double const value = self.x;
                return value;
            },
            [](vector_n& self, double const value) {
                self.x = value;
            }
        )
        .def_property("y",
        [](vector_n const& self) {
                T const value = self.y;
                return value;
            },
            [](vector_n& self, double const value) {
                self.y = value;
            }
        )
        .def_property("z",
        [](vector_n const& self) {
                T const value = self.z;
                return value;
            },
            [](vector_n& self, double const value) {
                self.z = value;
            }
        )
        // Formatted output
        .def("__str__", [](vector_n const& v) {
            return util::convertSpaceToNewLine(v.asString());
        })
        .def("__repr__", [](vector_n const& v) {
            return util::convertSpaceToNewLine(v.asString());
        })
        // Operations
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * T{})
        .def(py::self / T{})
        .def("dot", &vector_n::dot)
        .def("normalize", [](vector_n& v) { v.normalize(); return v; })
        .def("length", [](vector_n const& v) { return v.length(); })
        .def("length_sqr", [](vector_n const& v) { return v.lengthSquared(); })
        .def("projection", &vector_n::getVectorProjection);
    if constexpr (Size == 3) {
        vec_class
        .def(py::self * py::self);
    }
}