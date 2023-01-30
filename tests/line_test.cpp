#include "gtest/gtest.h"
#include "3dmath/Line.h"
#include "3dmath/Vector.h"
#include <vector>
#include <initializer_list>
#include <iostream>
#include <fstream>
using namespace std;
using namespace math3d;

TEST(Line, ImplicitForm) {

    // Build a line parallel to the x-axis and is at a distance of
    // 5 units from the x-axis
    Line2D<float> implictLine(0.0f, 1.0f, -5.0f);
    
    // Normal should be (0, 1) or (0, -1)
    Vector2D normal = implictLine.getNormal();
    ASSERT_FLOAT_EQ(normal.x, 0) << "Normal x-coordinate is incorrect";
    ASSERT_FLOAT_EQ(fabs(normal.y), 1.0) << "Normal y-coordinate is incorrect";

    // Direction should be (1,0) or (-1,0)
    Vector2D direction = implictLine.getDirection();
    ASSERT_FLOAT_EQ(fabs(direction.x), 1) << "Direction x-coordinate is incorrect";
    ASSERT_FLOAT_EQ(direction.y, 0) << "Direction y-coordinate is incorrect";

    // Point on line should be (0,5)
    // This will be the y-intercept of the line and chosen because x-intercept is infinity
    Vector2D ptOnLine = implictLine.getPointOnTheLine();
    ASSERT_FLOAT_EQ(ptOnLine.x, 0) << "Incorrect x-coordinate for point on the line";
    ASSERT_FLOAT_EQ(ptOnLine.y, 5) << "Incorrect y-coordinate for point on the line";

    // Assert parameteric form is correct
    ASSERT_EQ("[0,5]+t[-1,0]", implictLine.getParametericForm()) << "Parametric form of the line is wrong";

    // Assert implicit form is as per the input
    ASSERT_EQ("0x+1y-5=0", implictLine.getImplicitForm()) << "Implicit form of the line was incorrect";
}