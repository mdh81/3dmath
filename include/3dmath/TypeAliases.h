#pragma once
#include <vector>
#include <tuple>
#include "Vector.h"

namespace math3d::types {
    using Vertex = Vector3D<float>;
    using Point3D = Vertex;
    using Vector3D = Vector3D<float>;
    using Vertices = std::vector<Vertex>;
    using Tri = std::tuple<unsigned, unsigned, unsigned>;
    using Tris = std::vector<Tri>;
}