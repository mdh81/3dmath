#pragma once

#include "Primitive.h"
#include "../Vector.h"
#include <tuple>
#include <vector>

namespace math3d {
    // Simple convex shapes that have a well-defined center and
    // are represented using triangular faces
    class ConvexPrimitive : public Primitive {

    public:
        using Tri = std::tuple<unsigned, unsigned, unsigned>;
        using Tris = std::vector<Tri>;

    public:
        explicit ConvexPrimitive(Point const& center) // NOLINT
        : center(center) {
        }

        void orientTriangleNormalOutside(Tri& tri) {
            auto correctNormalOrientation = vertices.at(std::get<0>(tri)) - center;
            auto AB = vertices.at(std::get<1>(tri)) - vertices.at(std::get<0>(tri));
            auto AC = vertices.at(std::get<2>(tri)) - vertices.at(std::get<0>(tri));
            auto normal = AB * AC;
            if (correctNormalOrientation.dot(normal) < 0) {
                auto tmp = std::get<2>(tri);
                std::get<2>(tri) = std::get<1>(tri);
                std::get<1>(tri) = tmp;
            }
        }

        [[nodiscard]]
        Point getCenter() const {
            return center;
        }

        [[nodiscard]]
        Tris const& getTris() {
            return tris;
        }

    protected:
        Point center;
        Tris tris;
    };

}