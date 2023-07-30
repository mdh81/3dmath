#include "gtest/gtest.h"
#include "3dmath/RotationMatrix.h"
#include "3dmath/Utilities.h"
#include <iostream>
using namespace std;
using namespace math3d;

TEST(RotationMatrix, RotateAboutX) {
    auto rotateAboutXBy90 =
            RotationMatrix<float>::rotateAboutX(90);
    // ugh! c++ doesn't allow initializer list to be an argument to overloaded operators
    auto xAxisAfterRotation = rotateAboutXBy90.operator*({1, 0, 0, 0});
    ASSERT_TRUE(Utilities::areEqual(xAxisAfterRotation.dot({1, 0, 0, 0}), 1.f));

    auto yAxisAfterRotation = rotateAboutXBy90 * Vector<float, 4>{0, 1, 0, 0};
    ASSERT_TRUE(Utilities::areEqual(yAxisAfterRotation.dot({0, 0, 1, 0}), 1.f));

    auto zAxisAfterRotation = rotateAboutXBy90 * Vector<float, 4>{0, 0, 1, 0};
    ASSERT_TRUE(Utilities::areEqual(zAxisAfterRotation.dot({0, -1, 0, 0}), 1.f));

}

TEST(RotationMatrix, RotateAboutY) {
    auto rotateAboutYBy90 =
            RotationMatrix<float>::rotateAboutY(90);
    // ugh! c++ doesn't allow initializer list to be an argument to overloaded operators
    auto xAxisAfterRotation = rotateAboutYBy90.operator*({1, 0, 0, 0});
    ASSERT_TRUE(Utilities::areEqual(xAxisAfterRotation.dot({0, 0, -1, 0}), 1.f));

    auto yAxisAfterRotation = rotateAboutYBy90 * Vector<float, 4>{0, 1, 0, 0};
    ASSERT_TRUE(Utilities::areEqual(yAxisAfterRotation.dot({0, 1, 0, 0}), 1.f));

    auto zAxisAfterRotation = rotateAboutYBy90 * Vector<float, 4>{0, 0, 1, 0};
    ASSERT_TRUE(Utilities::areEqual(zAxisAfterRotation.dot({1, 0, 0, 0}), 1.f));
}

TEST(RotationMatrix, RotateAboutZ) {
    auto rotateAboutZBy90 =
            RotationMatrix<float>::rotateAboutZ(90);
    // ugh! c++ doesn't allow initializer list to be an argument to overloaded operators
    auto xAxisAfterRotation = rotateAboutZBy90.operator*({1, 0, 0, 0});
    ASSERT_TRUE(Utilities::areEqual(xAxisAfterRotation.dot({0, 1, 0, 0}), 1.f));

    auto yAxisAfterRotation = rotateAboutZBy90 * Vector<float, 4>{0, 1, 0, 0};
    ASSERT_TRUE(Utilities::areEqual(yAxisAfterRotation.dot({-1, 0, 0, 0}), 1.f));

    auto zAxisAfterRotation = rotateAboutZBy90 * Vector<float, 4>{0, 0, 1, 0};
    ASSERT_TRUE(Utilities::areEqual(zAxisAfterRotation.dot({0, 0, 1, 0}), 1.f));
}

TEST(RotationMatrix, RotateAboutArbitraryAxis) {

    Vector<double, 3>arbitraryAxis = Utilities::RandomVector();
    arbitraryAxis.normalize();

    // TODO: Allow a Vector4 to be created from Vector3
    Vector<double, 4> axis = {arbitraryAxis.x, arbitraryAxis.y, arbitraryAxis.z, 0};

    RotationMatrix<double> rotateAboutArbitraryVector(arbitraryAxis, Utilities::RandomNumber());

    auto axisAfterRotation = rotateAboutArbitraryVector * axis;
    ASSERT_TRUE(Utilities::areEqual(axisAfterRotation.dot(axis), 1.0));

}