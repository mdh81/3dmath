#include "pybind11/pybind11.h"

void bind_Vector3d(pybind11::module_ const&);

PYBIND11_MODULE(math3d, module) {
    auto const vectorModule = module.def_submodule("vector");
    bind_Vector3d(vectorModule);
}
