#pragma once

#include "Primitive.h"

namespace math3d {
    class Ray : public Primitive {
    public:
        Ray() = default;

        ~Ray() = default;

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