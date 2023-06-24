#pragma once
#include <memory>
#include <vector>
#include "../Vector.h"

namespace math3d {

    class Ray;

    class Primitive {

    public:
        using Vertex = Vector3D<float>;
        using Point = Vertex;
        using Vertices = std::vector<Vertex>;

    public:
        Primitive() = default;
        virtual ~Primitive() = default;
        virtual void generateGeometry() = 0;
        virtual IntersectionResult intersectWithRay(Ray const& ray) = 0;
        [[nodiscard]]
        std::vector<Vertex> const& getVertices() {
            return vertices;
        }

    protected:
       Vertices vertices;
    };

}
