#pragma once

#include "ConvexPrimitive.h"
#include "../PolarCoordinates.h"
#include "Ray.h"
#include <tuple>
#include <numbers>

namespace math3d {

    class Sphere : public ConvexPrimitive {

    public:
        Sphere(Point const& center, float radius, unsigned resolution = 16)
        : ConvexPrimitive(center)
        , radius(radius)
        , resolution(resolution) {

        }

        [[nodiscard]]
        Point getCenter() const {
            return origin;
        }

        [[nodiscard]]
        float getRadius() const {
            return radius;
        }

        [[nodiscard]]
        unsigned getResolution() const {
            return resolution;
        }

        void generateGeometry() override {
            if (!vertices.empty()) {
                std::cerr << "Warning: Skipping geometry generation. Geometry was already generated" << std::endl;
            }

            unsigned numCircles = resolution - 1;
            unsigned numCircleVertices = resolution;

            vertices.reserve((numCircleVertices * numCircles) + 2); // +2 for poles

            // Every pair of circles in the sphere will have resolution number of
            // quads in them. The two circles that join the poles will have resolution
            // number of triangles in them. Since we split quads into triangles we count
            // two faces for each quad
            unsigned numberOfQuads = resolution * numCircles - 1;
            unsigned numberOfTris = resolution * 2;
            tris.reserve((numberOfQuads * 2) + numberOfTris);

            // Generate vertices
            // We generate resolution number of circles in theta and phi directions. The first and last circles in the phi
            // direction are poles where the radius is zero. Phi will go from zero to 180 degrees, and theta will be a full
            // circle

            // Pole 1: At distance of radius from the center along +z
            vertices.emplace_back(origin +
                                  SphericalCoordinates{radius, 0.f, 0.f}.getCartesianCoordinates());

            // Circles between the two poles
            // In spherical coordinate system, phi = zero is a singularity, where theta has no influence on
            // the resulting coordinate. To prevent this, we start with phi at a sensible value given our resolution
            // constraint.
            float phiIncrement = constants::oneEightyDegreesInRadians / static_cast<float>(resolution);
            float thetaIncrement = constants::threeSixtyDegrees / static_cast<float>(resolution);
            float phi = phiIncrement, theta = 0.f;

            // Rotation by theta in spherical by convention rotates about the +z-axis. When theta is zero, the projection
            // of the spherical coordinate on the xy plane is on the +x-axis.
            // This coordinate (r, 0, phi) is our starting point for each circle. Each value of phi provides a new circle
            // and as theta varies from 0 to 360 we have new points on the chosen circle. All our circles are parallel
            // to the xy plane
            for (unsigned yzCircle = 1; yzCircle <= numCircles; ++yzCircle) {
                for (unsigned xyCircle = 1; xyCircle <= numCircleVertices; ++xyCircle) {
                    vertices.emplace_back(origin +
                                          SphericalCoordinates{radius, theta, phi}.getCartesianCoordinates());
                    theta += thetaIncrement;
                }
                theta = 0.f;
                phi += phiIncrement;
            }

            // Pole 2: At distance of radius from the center along -z
            vertices.emplace_back(origin +
                                  SphericalCoordinates{radius, 0.f, constants::oneEightyDegreesInRadians}.getCartesianCoordinates());

            // Generate faces
            // NOTE: In the two lambdas below, indices are vertex indices
            // quadNumber is [1, resolution] and it identifies the quadrilateral formed between corresponding segments of
            // two circles
            // circle1Index is the vertex index of the vertex on circle 1. circleIndex and circleIndex+1 define a segment on circle 1
            // circle2Index is the same as circle1Index but for circle 2
            auto createSplitQuad = [this](unsigned quadNumber, unsigned circle1Index, unsigned circle2Index) {
                // The quad is created between the pairs (circle1Index, circle1Index+1) and (circle2Index, circle2Index+1)
                // with consideration for the last segment in the circle, in which case, the second index in these two pairs
                // is subtracted from resolution to loop back to the first vertex in their respective circles
                bool isLastSegment = quadNumber == resolution;
                unsigned a = circle1Index;
                unsigned b = isLastSegment ? a + 1 - resolution : a + 1;
                unsigned c = circle2Index;
                unsigned d = isLastSegment ? c + 1 - resolution : c + 1;
                this->tris.emplace_back(orientTriangleNormalOutside({a, b, c}));
                this->tris.emplace_back(orientTriangleNormalOutside({b, c, d}));
            };

            // triangleNumber is [1...resolution], and it identifies the triangle that's constructed
            // poleIndex is the vertex index of the vertex at the poles, and it is either 0 or Number of Vertices-1
            // circleIndex is the vertex index of the vertex on the circle that is being joined with the pole
            auto createTri = [this](unsigned triangleNumber, unsigned poleIndex, unsigned circleIndex) {
                bool isLastSegment = triangleNumber == resolution;
                unsigned a = poleIndex;
                unsigned b = circleIndex;
                unsigned c = isLastSegment ? b + 1 - resolution : b + 1;
                this->tris.emplace_back(orientTriangleNormalOutside({a, b, c}));
            };

            // Faces between the first pole and circle
            unsigned nextIndex = 1;
            for (unsigned triangleNumber = 1; triangleNumber <= resolution; ++triangleNumber, ++nextIndex) {
                createTri(triangleNumber, 0, nextIndex);
            }

            // Faces between circles
            unsigned circle1VertexIndex = 1;
            unsigned circle2VertexIndex = circle1VertexIndex + resolution;
            for (unsigned circleNumber = 1; circleNumber < resolution - 1; ++circleNumber) {
                // Join corresponding edges to form a quad and then split that quad into two triangles
                for (unsigned quadNumber = 1; quadNumber <= resolution; ++quadNumber, ++circle1VertexIndex, ++circle2VertexIndex) {
                    createSplitQuad(quadNumber, circle1VertexIndex, circle2VertexIndex);
                }
            }

            // Faces between the second pole and final circle
            nextIndex = static_cast<unsigned>(vertices.size() - 1 - resolution);
            for (unsigned triangleNumber = 1; triangleNumber <= resolution; ++triangleNumber, ++nextIndex) {
                createTri(triangleNumber, vertices.size() - 1, nextIndex);
            }
        }

        IntersectionResult intersectWithRay(Ray const& ray) override {
            return {};
        }

    protected:
        float const radius;
        unsigned const resolution;
    };
}
