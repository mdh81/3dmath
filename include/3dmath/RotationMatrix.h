#pragma once
#include "IdentityMatrix.h"
#include "Utilities.h"
namespace math3d {

// 3x3 column-major rotation matrix
template<typename DataType>
class RotationMatrix : public IdentityMatrix<DataType, 4, 4> {

public:
    // TODO: Add derivation
    RotationMatrix(Vector3<DataType> const& rotationAxis,
                   DataType const rotationInDegrees) {

        auto cosTheta = static_cast<DataType>(cos(Utilities::asRadians(rotationInDegrees)));
        auto oneMinusCosTheta = 1 - cosTheta;
        auto sinTheta = static_cast<DataType>(sin(Utilities::asRadians(rotationInDegrees)));

        // TODO: Add a specialization that allows assigning a vec3 to a vec4 with w set to 0 automatically
        this->operator[](0) =
            {((rotationAxis.x * rotationAxis.x) * oneMinusCosTheta) + cosTheta,
             ((rotationAxis.x * rotationAxis.y) * oneMinusCosTheta) + (rotationAxis.z * sinTheta),
             ((rotationAxis.x * rotationAxis.z) * oneMinusCosTheta) - (rotationAxis.y * sinTheta),
             0};

        this->operator[](1) =
             {((rotationAxis.x * rotationAxis.y) * oneMinusCosTheta) - (rotationAxis.z * sinTheta),
              ((rotationAxis.y * rotationAxis.y) * oneMinusCosTheta) + cosTheta,
              ((rotationAxis.z * rotationAxis.y) * oneMinusCosTheta) + (rotationAxis.x * sinTheta),
              0};

        this->operator[](2) =
            {((rotationAxis.x * rotationAxis.z) * oneMinusCosTheta) + (rotationAxis.y * sinTheta),
             ((rotationAxis.y * rotationAxis.z) * oneMinusCosTheta) - (rotationAxis.x * sinTheta),
             ((rotationAxis.z * rotationAxis.z) * oneMinusCosTheta) + cosTheta,
             0};
    }

    // See https://github.com/mdh81/3dmath/blob/master/derivations/Rotation_About_X.jpg
    static RotationMatrix rotateAboutX(DataType const rotationInDegrees) {
        return RotationMatrix({1, 0, 0}, rotationInDegrees);
    }

    // See https://github.com/mdh81/3dmath/blob/master/derivations/Rotation_About_Y.jpg
    static RotationMatrix rotateAboutY(DataType const rotationInDegrees) {
        return RotationMatrix({0, 1, 0}, rotationInDegrees);
    }

    // See https://github.com/mdh81/3dmath/blob/master/derivations/Rotation_About_Z.jpg
    static RotationMatrix rotateAboutZ(DataType const rotationInDegrees) {
       return RotationMatrix({0, 0, 1}, rotationInDegrees);
    }

private:
    // To allow static member functions to create an empty matrix and to set
    // the data explicitly
    RotationMatrix() = default;

};

}
