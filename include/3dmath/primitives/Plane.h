#pragma once

#include "ConvexPrimitive.h"
#include "Ray.h"
#include <iostream>

namespace math3d {

    class Plane : public ConvexPrimitive {

    public:
        Plane(types::Point3D const& point, types::Vector3D const& normal, unsigned geometrySize = 1)
        : ConvexPrimitive(point)
        , normal(normal)
        , geometrySize(geometrySize) {
            this->normal.normalize();
        }

        [[nodiscard]]
        types::Vector3D getNormal() const {
            return normal;
        }

        // TODO: Sketch this in README
        void generateGeometry() override {

            if (!vertices.empty()) {
                std::cerr << "Warning: Skipping geometry generation. Geometry was already generated" << std::endl;
            }

            // Construct a vector that is perpendicular to the normal and call
            // it the horizontal axis of the plane
            Vector horizontalAxis = Utilities::getPerpendicular(normal).normalize();

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

        // Compute projection of the given point on this plane
        types::Point3D getProjection(types::Point3D const& point) {
            auto planeOriginToPoint = point - origin;
            auto distanceToPlane = planeOriginToPoint.dot(normal);
            return point + (distanceToPlane * -normal);
        }

        // TODO: Sketch this
        IntersectionResult intersectWithRay(const Ray &ray) override {
            // A ray and a plane cannot intersect when the ray is parallel to the plane
            // and the ray origin is not on the plane. Otherwise, the ray will
            // intersect the plane
            IntersectionResult result;
            result.status = IntersectionStatus::Intersects;

            auto isParallel = Utilities::isZero(ray.getDirection().dot(normal));

            // Project the vector from ray to plane origin onto the plane normal
            // to get the distance of the ray origin from the plane
            auto planeToRayOrigin = ray.getOrigin() - origin;
            auto distanceToRayOrigin = planeToRayOrigin.dot(normal);

            if (Utilities::isZero(distanceToRayOrigin)) {
                // If the ray origin is on the plane, then the ray origin is the intersection point
                result.intersectionPoint = ray.getOrigin();
            } else if (!isParallel) {
                // If the ray origin is not on the plane, and the plane and ray are not parallel, then there is
                // a true intersection at distance t from the ray origin
                auto t = (origin - ray.getOrigin()).dot(normal) /
                              ray.getDirection().dot(normal);
                result.intersectionPoint = ray.getOrigin() + t * ray.getDirection();
            } else {
                // If the ray origin is not on the plane, and the ray and plane are parallel, then there is
                // no intersection
                result.status = IntersectionStatus::NoIntersection;
            }
            return result;
        }

    private:
        types::Vector3D normal;
        unsigned const geometrySize;

    friend std::ostream& operator<<(std::ostream& os, Plane const& plane);
    };

    inline std::ostream& operator<<(std::ostream& os, Plane const& plane) {
        os << "Origin: " << plane.getOrigin() << " Normal: " << plane.getNormal();
        return os;
    }
}
