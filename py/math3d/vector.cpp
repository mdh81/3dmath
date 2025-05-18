#include "pybind11/pybind11.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"

#include "Vector.h"

namespace py = pybind11;

void bind_Vector3d(py::module_ const& module) {
    using vector3d = math3d::Vector<double, 3>;
    py::class_<vector3d>(module, "Vector3d")
        // Construction
        .def(py::init())
        .def(py::init([](py::list const& list) {
            auto const input = list.cast<std::vector<double>>();
            return vector3d(input);
        }))
        // Member access
        .def_property("x",
            [](vector3d const& self) {
                double const value = self.x;
                return value;
            },
            [](vector3d& self, double const value) {
                self.x = value;
            }
        )
        .def_property("y",
        [](vector3d const& self) {
                double const value = self.y;
                return value;
            },
            [](vector3d& self, double const value) {
                self.y = value;
            }
        )
        .def_property("z",
        [](vector3d const& self) {
                double const value = self.z;
                return value;
            },
            [](vector3d& self, double const value) {
                self.z = value;
            }
        )
        // Formatted output
        .def("__str__", &vector3d::asString)
        .def("__repr__", &vector3d::asString)
        // Operations
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self * double{})
        .def(py::self / double{})
        .def("dot", &vector3d::dot)
        .def("normalize", [](vector3d& v) { v.normalize(); return v; })
        .def("length", [](vector3d const& v) { return v.length(); })
        .def("length_sqr", [](vector3d const& v) { return v.lengthSquared(); })
        .def("projection", &vector3d::getVectorProjection);
}