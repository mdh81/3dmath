#pragma once

#include "ConvexPrimitive.h"
#include "Ray.h"
#include <iostream>

namespace math3d {

    class Plane : public ConvexPrimitive {

    public:
        Plane(Point const& point, Vector const& normal, unsigned geometrySize = 10)
        : ConvexPrimitive(point)
        , normal(normal)
        , geometrySize(geometrySize) {
            this->normal.normalize();
        }

        [[nodiscard]]
        Vector getNormal() const {
            return normal;
        }

        // TODO: Sketch this in README
        void generateGeometry() override {

            if (!vertices.empty()) {
                std::cerr << "Warning: Skipping geometry generation. Geometry was already generated" << std::endl;
            }

            // Construct a vector that is perpendicular to the normal and call
            // it the horizontal axis of the plane
            Vector horizontalAxis = getPerpendicular(normal);

            // Construct the vertical axis as the perpendicular of the horizontal
            // axis
            Vector verticalAxis = normal * horizontalAxis;

            // Create a quad
            // Upper right corner
            auto halfSize = geometrySize * 0.5f;
            vertices.emplace_back(origin +
                                 (halfSize * horizontalAxis) +
                                 (halfSize * verticalAxis));
            // Upper left corner
            vertices.emplace_back(origin -
                                 (halfSize * horizontalAxis) +
                                 (halfSize * verticalAxis));

            // Lower left corner
            vertices.emplace_back(origin -
                                  (halfSize * horizontalAxis) -
                                  (halfSize * verticalAxis));

            // Lower right corner
            vertices.emplace_back(origin +
                                  (halfSize * horizontalAxis) -
                                  (halfSize * verticalAxis));

            // Split the quad into two tris
            tris.emplace_back(0, 1, 2);
            tris.emplace_back(2, 3, 0);
        }

        IntersectionResult intersectWithRay(const Ray &ray) override {
            return IntersectionResult();
        }

    private:
        Vector normal;
        unsigned const geometrySize;
    };
}
