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
    using Vector2D = Vector2D<float>;
    using Point2D = Point2D<float>;

    // Build a line parallel to the x-axis and is at a distance of
    // 5 units from the x-axis
    ImplicitLine2D<float> implicitLine(0.0f, 1.0f, -5.0f);

    // Normal should be (0, 1)
    Vector2D normal = implicitLine.getNormal();
    ASSERT_FLOAT_EQ(normal.x, 0) << "Normal x-coordinate is incorrect";
    ASSERT_FLOAT_EQ(normal.y, 1.0) << "Normal y-coordinate is incorrect";

    // Direction should be (1,0) or (-1,0)
    Vector2D direction = implicitLine.getDirection();
    ASSERT_FLOAT_EQ(fabs(direction.x), 1) << "Direction x-coordinate is incorrect";
    ASSERT_FLOAT_EQ(direction.y, 0) << "Direction y-coordinate is incorrect";

    // Point on line should be (0,5)
    // This will be the y-intercept of the line and chosen because x-intercept is infinity
    Vector2D ptOnLine = implicitLine.getPointOnTheLine();
    ASSERT_FLOAT_EQ(ptOnLine.x, 0) << "Incorrect x-coordinate for point on the line";
    ASSERT_FLOAT_EQ(ptOnLine.y, 5) << "Incorrect y-coordinate for point on the line";

    // Assert parametric form is correct
    ASSERT_EQ("[0,5]+t[-1,0]", implicitLine.getParametric().toString()) << "Parametric form of the line is wrong";

    // Assert implicit form is as per the input
    ASSERT_EQ("0x+1y-5=0", implicitLine.toString()) << "Implicit form of the line was incorrect";


    // Assert E1 intercept is infinity
    Point2D e1Int = implicitLine.getXIntercept();
    ASSERT_EQ(std::numeric_limits<float>::infinity(), e1Int.x) << "Line is parallel to x. "
                                                                  "Expecting intersection with x-axis at infinity";
    ASSERT_FLOAT_EQ(0, e1Int.y) << "Y coordinate of x-intercept should be 0";

    // Assert E2 intercept is (0,5)
    Point2D e2Int = implicitLine.getYIntercept();
    ASSERT_FLOAT_EQ(0, e2Int.x) << "Line is parallel to x. x-intercept should be 0";
    ASSERT_FLOAT_EQ(5, e2Int.y) << "Line is parallel to x and is 5 units away from x-axis. "
                                   "Y coordinate of x-intercept should be 5";
}

TEST(Line, ParametricForm) {

    ParametricLine2D<float> parametricLine({0,5}, {1,0});

    ASSERT_FLOAT_EQ(parametricLine.getNormal().x, 0) << "Line is parallel to x-axis. "
                                                        "x-coordinate of normal should be zero";
    ASSERT_FLOAT_EQ(fabs(parametricLine.getNormal().y), 1) << "Line is parallel to x-axis. "
                                                                     "y-coordinate of normal should be one";

    // Add test for conversion to implicit
    ParametricLine2D<float> parametricLine1({2,5,}, {3,-3});
    auto implicitLine = parametricLine1.getImplicit();
    ASSERT_EQ(implicitLine.toString(), "3x+3y-21=0");

}

TEST(Line, ClosestPointParametric) {
    ParametricLine2D<float> parametricLine({0,0}, {1,0});
    Point2D<float> pointNotOnLine{5,5};
    auto closestPoint = parametricLine.closestPoint(pointNotOnLine);
    ASSERT_FLOAT_EQ(5, closestPoint.x) << "x-coordinate of closest point is wrong";
    ASSERT_FLOAT_EQ(0, closestPoint.y) << "x-coordinate of closest point is wrong";
    Point2D<float> pointOnLine{0,0};
    closestPoint = parametricLine.closestPoint(pointOnLine);
    ASSERT_FLOAT_EQ(0, (closestPoint - pointOnLine).length()) << "Closest point calculation is wrong when input is a "
                                                                 "point on the line";
}