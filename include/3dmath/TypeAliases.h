#pragma once
#include <vector>
#include <tuple>
#include "Vector.h"

namespace math3d::types {
    using Point3D = Vector3<double>;
    using Vector3D = Vector3<double>;
    using Vertex = Vector3<float>;
    using Vertices = std::vector<Vertex>;
    using Tri = std::tuple<unsigned, unsigned, unsigned>;
    using Tris = std::vector<Tri>;
}