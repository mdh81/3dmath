#pragma once

#include "pybind11/pybind11.h"

#include "SupportingTypes.h"

namespace py = pybind11;
namespace m3d = math3d;

template<typename T>
void bind_Extent(py::module_ const& module, std::string_view className) {
    using Extent = m3d::Extent<T>;
    py::class_<Extent>(module, className.data())
    .def(py::init([] {
        Extent extent;
        extent.min = std::numeric_limits<T>::max();
        extent.max = -std::numeric_limits<T>::max();
        return extent;
    }))
    .def(py::init([](T min, T max) {
        Extent extent {};
        extent.min = min;
        extent.max = max;
        return extent;
    }))
    .def_property("min",
        [](Extent const& extent) {
            return extent.min;
        },
        [](Extent& extent, T min) {
           extent.min = min;
        }
    )
    .def_property("max",
        [](Extent const& extent) {
            return extent.max;
        },
        [](Extent& extent, T max) {
           extent.max = max;
        }
    )
    .def("length", [](Extent const& extent) {
        return extent.length();
    })
    .def("center", [](Extent const& extent) {
        return extent.center();
    })
    .def("update", [](Extent& extent, T val) {
        extent.min = std::min(extent.min, val);
        extent.max = std::max(extent.max, val);
    })
    .def("__str__", [](Extent const& extent) {
        return extent.asString();
    })
    .def("__repr__", [](Extent const& extent) {
        return std::format("Extent = {} Valid = {}", extent.asString(), extent.min < extent.max);
    });
}

template<typename T>
void bind_Bounds(py::module_ const& module, std::string_view className) {
    using Bounds = m3d::Bounds3D<T>;
    py::class_<Bounds>(module, className.data())
    .def(py::init())
    .def(py::init([](m3d::Extent<T> const& x, m3d::Extent<T> const& y, m3d::Extent<T> const& z) {
        return Bounds {x, y, z};
    }))
    .def(py::init([](m3d::Vector3<T> const& min, m3d::Vector3<T> const& max) {
        return Bounds(m3d::Extent<T>{min.x, max.x}, m3d::Extent<T>{min.y, max.y}, m3d::Extent<T>{min.z, max.z});
    }))
    .def("min", [](Bounds const& self) {
        return self.min();
    })
    .def("max", [](Bounds const& self) {
        return self.max();
    })
    .def("corners", [](Bounds const& self) {
        return self.corners();
    })
    .def("edges", [](Bounds const& self) {
        return self.edges();
    })
    .def("merge", [](Bounds& self, Bounds const& another) {
        self.merge(another);
    })
    .def("x_extent", [](Bounds const& self) {
        return self.extent(Bounds::Direction::x);
    })
    .def("y_extent", [](Bounds const& self) {
        return self.extent(Bounds::Direction::y);
    })
    .def("z_extent", [](Bounds const& self) {
        return self.extent(Bounds::Direction::z);
    })
    .def("__str__", [](Bounds const& bounds) {
        return bounds.asString();
    })
    .def("__repr__", [](Bounds const& bounds) {
        return std::format(
            "{} Valid = {}\n"
            "Center = {}\n"
            "X Length = {}\n"
            "Y Length = {}\n"
            "Z Length = {}\n"
            "Diagonal Length {}", bounds.asString(), bounds.isValid(), bounds.center().asString(),
            bounds.x.length(), bounds.y.length(), bounds.z.length(), bounds.length());
    });
}