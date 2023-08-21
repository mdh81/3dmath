#pragma once

#include "IdentityMatrix.h"
#include "SupportingTypes.h"
namespace math3d {

template<typename DataType>
class OrthographicProjectionMatrix : public IdentityMatrix<DataType, 4u, 4u> {
    static_assert(std::is_same<float, DataType>() ||
                  std::is_same<double, DataType>(), "Float and double are the only allowed types");
private:
    using Matrix<DataType, 4u, 4u>::data;
public:
    // Allow users to create an empty matrix and then call update when the projection needs to be recomputed
    OrthographicProjectionMatrix() = default;

    // OpenGL uses a left-handed system for normalized device coordinates, so z has to be inverted
    OrthographicProjectionMatrix(Bounds3D<DataType> const& bounds3D, bool invertZ = true) {
        update(bounds3D, invertZ);
    }

    void update(Bounds3D<DataType> const& bounds3D, bool invertZ = true) {
        // Bounding box has to have valid extents
        if (isZero(bounds3D.x) || isZero(bounds3D.y) || isZero(bounds3D.z)) {
            throw std::runtime_error("Unable to compute orthographic projection: Invalid bounds!");
        }

        // See https://mdh81.github.io/3dmath/orthographicProjection/ for a derivation for this matrix
        data[0]  = 2 / bounds3D.x.length();
        data[5]  = 2 / bounds3D.y.length();
        data[10] = (2 / bounds3D.z.length()) * (invertZ ? -1 : 1);
        data[12] = -(bounds3D.x.min + bounds3D.x.max) / bounds3D.x.length();
        data[13] = -(bounds3D.y.min + bounds3D.y.max) / bounds3D.y.length();
        data[14] = -(bounds3D.z.min + bounds3D.z.max) / bounds3D.z.length();
    }

};

}

