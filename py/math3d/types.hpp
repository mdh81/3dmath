#pragma once

#include "pybind11/pybind11.h"

#include "SupportingTypes.h"

namespace py = pybind11;
namespace m3d = math3d;

template<typename T>
void bind_Extent(py::module_ const& module, std::string_view className) {
    using Extent = m3d::Extent<T>;
    py::class_<Extent>(module, className.data())
    .def(py::init([](T min, T max) {
        Extent extent {};
        extent.min = min;
        extent.max = max;
        return extent;
    }))
    .def("length", [](Extent const& extent) {
        return extent.length();
    })
    .def("center", [](Extent const& extent) {
        return extent.center();
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