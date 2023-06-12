#pragma once

#include "ConvexPrimitive.h"
#include "../OtherTypes.h"
#include <tuple>
#include <numbers>

namespace math3d {

    class Sphere : public ConvexPrimitive {

    public:
        Sphere(Point const& center, float radius)
        : ConvexPrimitive(center)
        , radius(radius)
        , resolution(16)
        , subdivisions(16) {

        }

        [[nodiscard]]
        float getRadius() const {
            return radius;
        }

        [[nodiscard]]
        unsigned getResolution() const {
            return resolution;
        }

        [[nodiscard]]
        unsigned getSubdivisions() const {
            return subdivisions;
        }

        void generateGeometry() override {
            if (!vertices.empty()) {
                std::cerr << "Warning: Skipping geometry generation. Geometry was already generated" << std::endl;
            }
            // Except the poles, each circle in the sphere will
            // have resolution number of vertices. The two poles have a vertex each.
            vertices.reserve((resolution * (subdivisions - 1)) + 2);

            // Each pair of circles in the sphere will have resolution number of
            // quads in them. The two circles that join the poles will have resolution
            // number of triangles in them. Since we split quads into triangles we count
            // two faces for each quad
            tris.reserve((resolution * 2 * subdivisions - 1) + (resolution * 2));

            // Generate vertices
            float twoPI = 2*std::numbers::pi;
            float diameter = 2*radius;
            float increment = diameter / static_cast<float>(subdivisions);
            float z = -radius;
            for (unsigned circleNumber = 1; circleNumber <= subdivisions + 1; ++circleNumber, z+=increment) {
                float circleRadius = radius - fabs(z);
                if (fabs(circleRadius) < 1e-6) { // Pole
                    vertices.emplace_back(center + (Vector3D<float>{0, 0, 1} * z));
                } else {                        // Circle
                    float theta = 0.f;
                    float thetaIncrement = twoPI / static_cast<float>(resolution);
                    for (unsigned i = 0; i < resolution; ++i) {
                        vertices.emplace_back(center + Vector3D<float>{ radius * cos(theta), radius * sin(theta), z });
                        theta += thetaIncrement;
                    }
                }
            }

            // Generate faces
            unsigned vertexIndex = 0;
            for (unsigned circleNumber = 2; circleNumber <= subdivisions; ++circleNumber) {
                // Create triangles between neighboring circles
            }
            // Create triangles between poles and neighboring circles
            // Pole 1
            unsigned circleVertexIndex = 1;
            for (unsigned triangleNumber = 1; triangleNumber <= resolution; ++triangleNumber) {

            }
            // Pole 2
            circleVertexIndex = static_cast<unsigned>(vertices.size() - 1);
            for (unsigned triangleNumber = 1; triangleNumber <= resolution; ++triangleNumber) {

            }
        }

        Vector3D<float> intersectWithRay(Ray const& ray) override {
            return {0, 0, 0};
        }

    private:
        float const radius;
        unsigned const resolution;
        unsigned const subdivisions;
    };
}
