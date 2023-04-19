#pragma once

#include "IdentityMatrix.h"
#include "OtherTypes.h"
namespace math3d {

template<typename DataType>
class OrthographicProjectionMatrix : public IdentityMatrix<DataType, 4u, 4u> {
    static_assert(std::is_same<float, DataType>() ||
                  std::is_same<double, DataType>(), "Float and double are the only allowed types");
private:
    using Matrix<DataType, 4u, 4u>::m_data;
public:
    // OpenGL uses a left-handed system for normalized device coordinates, so z has to be inverted
    OrthographicProjectionMatrix(Bounds3D<DataType> const& bounds3D, bool invertZ = true) {
        // Bounding box has to have valid extents
        if (isZero(bounds3D.x) || isZero(bounds3D.y) || isZero(bounds3D.z)) {
            throw std::runtime_error("Unable to compute orthographic projection: Invalid bounds!");
        }

        // Refer to
        // https://github.com/mdh81/3dmath/blob/master/README.md#orthographic-projection-matrix
        // for a derivation of this matrix

        m_data[0]  =  2 / bounds3D.x.length();
        m_data[5]  =  2 / bounds3D.y.length();
        m_data[10] = (2 / bounds3D.z.length()) * (invertZ ? -1 : 1);
        m_data[12] = -(bounds3D.x.min + bounds3D.x.max) / bounds3D.x.length();
        m_data[13] = -(bounds3D.y.min + bounds3D.y.max) / bounds3D.y.length();
        m_data[14] = -(bounds3D.z.min + bounds3D.z.max) / bounds3D.z.length();
    }

};

}

