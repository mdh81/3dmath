#pragma once
#include "IdentityMatrix.h"
namespace math3d {

// 3x3 column-major rotation matrix
template<typename DataType>
class RotationMatrix : public IdentityMatrix<DataType, 3, 3> {

};

}
