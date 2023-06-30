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
            // Initialize result
            IntersectionResult result;
            result.status = IntersectionStatus::NoIntersection;

            // No intersection if rays are parallel
            auto d1xd2 = this->direction * ray.direction;
            if (!Utilities::isZero(d1xd2.length())) {

                // Parametric distance along ray to the intersection point
                auto d1xd2Length = d1xd2.length();
                float t = ((ray.getOrigin() - origin) * ray.getDirection()).dot(d1xd2) / (d1xd2Length * d1xd2Length);

                // Intersection that occurs behind the ray origin is not a valid intersection
                if (t > 0 || Utilities::isZero(t)) {

                    result.intersectionPoint = origin + (t * direction);

                    // For the intersection to be valid, the intersection point should be on the ray, otherwise
                    // the two rays are on parallel planes
                    if (Utilities::isZero(this->distanceToPoint(result.intersectionPoint))) {
                        result.status = IntersectionStatus::Intersects;
                    } else {
                        result.status = IntersectionStatus::Skew;
                    }
                }
            }

            return result;
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

        void writeToFile(const std::filesystem::path &outputFile) override {


        }

    private:
        Point origin;
        Vector direction;
    };
}