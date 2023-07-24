#pragma once
#include "IdentityMatrix.h"
#include "Utilities.h"
namespace math3d {

// 3x3 column-major rotation matrix
template<typename DataType>
class RotationMatrix : public IdentityMatrix<DataType, 4, 4> {

public:
    RotationMatrix(Vector3<DataType> const& rotationAxis,
                   DataType const rotationInDegrees) {

    }

    // See https://github.com/mdh81/3dmath/blob/master/derivations/Rotation_About_X.jpg
    static RotationMatrix rotateAboutX(DataType const rotationInDegrees) {
        auto cosTheta = cos(Utilities::asRadians(rotationInDegrees));
        auto sinTheta = sin(Utilities::asRadians(rotationInDegrees));
        auto result = RotationMatrix();
        result[1] = {0,  cosTheta, sinTheta, 0};
        result[2] = {0, -sinTheta, cosTheta, 0};
        return result;
    }

    // See https://github.com/mdh81/3dmath/blob/master/derivations/Rotation_About_Y.jpg
    static RotationMatrix rotateAboutY(DataType const rotationInDegrees) {
        auto cosTheta = cos(Utilities::asRadians(rotationInDegrees));
        auto sinTheta = sin(Utilities::asRadians(rotationInDegrees));
        auto result = RotationMatrix();
        result[0] = {cosTheta, 0, -sinTheta, 0};
        result[2] = {sinTheta, 0,  cosTheta, 0};
        return result;
    }

    // See https://github.com/mdh81/3dmath/blob/master/derivations/Rotation_About_Z.jpg
    static RotationMatrix rotateAboutZ(DataType const rotationInDegrees) {
        auto cosTheta = cos(Utilities::asRadians(rotationInDegrees));
        auto sinTheta = sin(Utilities::asRadians(rotationInDegrees));
        auto result = RotationMatrix();
        result[0] = { cosTheta, sinTheta, 0, 0};
        result[1] = {-sinTheta, cosTheta, 0, 0};
        return result;
    }

private:
    // To allow static member functions to create an empty matrix and to set
    // the data explicitly
    RotationMatrix() = default;

};

}
