#include "pybind11/pybind11.h"
#include "vector.hpp"

PYBIND11_MODULE(math3d, module) {
    auto const vectorModule = module.def_submodule("vector");
    bind_Vector<double, 3>(vectorModule, "vector3");
    bind_Vector<double, 4>(vectorModule, "vector4");
    bind_Vector<double, 2>(vectorModule, "vector2");
    bind_Vector<float, 3>(vectorModule, "vector3f");
    bind_Vector<float, 4>(vectorModule, "vector4f");
    bind_Vector<float, 2>(vectorModule, "vector2f");
}
