#pragma once
#include <memory>
#include <vector>
#include <filesystem>
#include "../Vector.h"
#include "../SupportingTypes.h"

namespace math3d {

    class Ray;

    class Primitive {

    public:
        using Vertex = Vector3D<float>;
        using Point = Vertex;
        using Vector = Point;
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
        virtual void writeToFile(std::filesystem::path const& outputFile) = 0;

    protected:
       Vertices vertices;
    };

}
