#pragma once

#include "Primitive.h"
#include "../SupportingTypes.h"
#include "../Utilities.h"

namespace math3d {
    // Ray has an origin and a direction. The direction is normalized and the ray is
    // assumed to extend to infinity from the origin
    class Ray : public Primitive {
    public:
        Ray(Point const& origin, Vector const& direction)
        : origin(origin)
        , direction(direction) {
            this->direction.normalize();
        }

        ~Ray() = default;

        // See https://github.com/mdh81/3dmath/blob/master/derivations/PointDistanceToRay.jpg
        [[nodiscard]]
        float distanceToPoint(Point const& point) const {
            auto v = point - origin;
            auto lengthOfV = v.length();
            float angle = acos(v.dot(direction) / lengthOfV);
            return lengthOfV * sin(angle) ;
        }

        // Please refer to
        // https://github.com/mdh81/3dmath/raw/master/derivations/RayRayIntersection.jpg
        IntersectionResult intersectWithRay(Ray const& ray) override {
            IntersectionResult result;

            // Check if rays are parallel
            auto d1xd2 = this->direction * ray.direction;
            if (Utilities::isZero(d1xd2.length())) {
                result.status = IntersectionStatus::NoIntersection;
            }

            // Parametric distance along ray to the intersection point
            auto d1xd2Length = d1xd2.length();
            float t = ((ray.getOrigin() - origin).dot(d1xd2)) / (d1xd2Length * d1xd2Length);

            // Intersection point
            result.intersectionPoint = origin + (t * direction);

            // Check if the intersection point is on the line by computing the perpendicular distance between this ray
            // and the point

            return {};
        }

        [[nodiscard]]
        Vector getDirection() const {
            return direction;
        }

        [[nodiscard]]
        Vector3D<float> getOrigin() const {
            return origin;
        }

        void generateGeometry() override {
        }

    private:
        Point origin;
        Vector direction;
    };
}