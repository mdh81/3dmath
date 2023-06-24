#pragma once

#include "Primitive.h"

namespace math3d {
    // Ray has an origin and a direction. The direction is normalized and the ray is
    // assumed to extend to infinity from the origin
    class Ray : public Primitive {
    public:
        Ray() = default;

        ~Ray() = default;


        // Please refer to
        Point intersectWithRay(const Ray &ray) override {
            return {};
        }

        [[nodiscard]]
        Vector3D<float> const &getDirection() const {
            return direction;
        }

        [[nodiscard]]
        Vector3D<float> const &getOrigin() const {
            return vertices.at(0);
        }

    private:
        Vector3D<float> direction;
    };
}