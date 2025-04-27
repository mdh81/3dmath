#include "3dmath/Quaternion.h"
#include "gtest/gtest.h"
#include "3dmath/Utilities.h"

TEST(Quaternion, Magnitude) {
    math3d::Quaternion<double> const quaternion {math3d::Utilities::RandomNumber{}, math3d::Utilities::RandomVector{}};
    ASSERT_DOUBLE_EQ(quaternion.magnitude(), 1) << "Rotation quaternion's magnitude should have been 1" << std::endl;
}

TEST(Quaternion, Conjugate) {
    math3d::Quaternion<double> const quaternion {math3d::Utilities::RandomNumber{}, math3d::Utilities::RandomVector{}};
    auto conjugate = quaternion.conjugate();
    ASSERT_DOUBLE_EQ(conjugate.axis().x, -quaternion.axis().x);
    ASSERT_DOUBLE_EQ(conjugate.axis().y, -quaternion.axis().y);
    ASSERT_DOUBLE_EQ(conjugate.axis().z, -quaternion.axis().z);
    ASSERT_DOUBLE_EQ(conjugate.angle(), quaternion.angle());
}

TEST(Quaternion, Inverse) {

}


