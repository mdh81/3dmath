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
    ImplicitLine2D<float> implictLine(0.0f, 1.0f, -5.0f);

    using Vector2D = Vector2D<float>;
    
    // Normal should be (0, 1)
    Vector2D normal = implictLine.getNormal();
    ASSERT_FLOAT_EQ(normal.x, 0) << "Normal x-coordinate is incorrect";
    ASSERT_FLOAT_EQ(normal.y, 1.0) << "Normal y-coordinate is incorrect";

    // Direction should be (1,0) or (-1,0)
    Vector2D direction = implictLine.getDirection();
    ASSERT_FLOAT_EQ(fabs(direction.x), 1) << "Direction x-coordinate is incorrect";
    ASSERT_FLOAT_EQ(direction.y, 0) << "Direction y-coordinate is incorrect";
/*
    // Point on line should be (0,5)
    // This will be the y-intercept of the line and chosen because x-intercept is infinity
    Vector2D ptOnLine = implictLine.getPointOnTheLine();
    ASSERT_FLOAT_EQ(ptOnLine.x, 0) << "Incorrect x-coordinate for point on the line";
    ASSERT_FLOAT_EQ(ptOnLine.y, 5) << "Incorrect y-coordinate for point on the line";

    // Assert parameteric form is correct
    ASSERT_EQ("[0,5]+t[-1,0]", implictLine.getParametric().toString()) << "Parametric form of the line is wrong";

    // Assert implicit form is as per the input
    ASSERT_EQ("0x+1y-5=0", implictLine.toString()) << "Implicit form of the line was incorrect";


    // Assert E1 intercept is infinity
    Vector2D e1Int = implictLine.getXIntercept();
    ASSERT_EQ(std::numeric_limits<float>::infinity(), e1Int.x) << "Line is parallel to x. "
                                                                  "Expecting intersection with x-axis at infinity";
    ASSERT_FLOAT_EQ(0, e1Int.y) << "Y coordinate of x-intercept should be 0";

    // Assert E2 intercept is (0,5)
    Vector2D e2Int = implictLine.getYIntercept();
    ASSERT_FLOAT_EQ(0, e2Int.x) << "Line is parallel to x. x-intercept should be 0";
    ASSERT_FLOAT_EQ(5, e2Int.y) << "Line is parallel to x and is 5 units away from x-axis. "
                                   "Y coordinate of x-intercept should be 5";*/
}

TEST(Line, ParametricForm) {

    // Define a line in parametric form
   // ParametricLine2D<float> parametricLine({0,5}, {1,0});
/*
    // Assert normal is either (0,1) or (0,-1)
    ASSERT_FLOAT_EQ(parametricLine.getNormal().x, 0) << "Line is parallel to x-axis. "
                                                        "x-coordinate of normal should be zero";

    ASSERT_FLOAT_EQ(fabs(parametricLine.getNormal().y), 0) << "Line is parallel to x-axis. "
                                                              "x-coordinate of normal should be zero";
                                                              */

}