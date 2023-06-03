#include "gtest/gtest.h"
#include "3dmath/Matrix.h"
#include "3dmath/IdentityMatrix.h"
#include "3dmath/OrthographicProjectionMatrix.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;
using namespace math3d;

TEST(Matrix, DefaultConstruction) {
    // Assert all matrix entries are zeroes
    Matrix<float, 10, 10> m; 
    ASSERT_EQ(m.getNumberOfRows(), 10);
    ASSERT_EQ(m.getNumberOfColumns(), 10);
    float const* data = m.getData();
    for (int i = 0; i < 99; ++i) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, Identity) {
    
    // Assert identity matrix is a diagonal matrix
    IdentityMatrix<float, 3, 3> m1; 
    float const* data = m1.getData();
    for (auto i : {0,4,8}) {
        ASSERT_FLOAT_EQ(1, data[i]);
    }
    for (auto i : {1,2,3,5,6,7}) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, CopyConstruction) {
    
    IdentityMatrix<float, 3, 3>m1;
    IdentityMatrix<float, 3, 3>m2(m1);
    float const* data = m2.getData();
    for (auto i : {0,4,8}) {
        ASSERT_FLOAT_EQ(1, data[i]);
    }
    for (auto i : {1,2,3,5,6,7}) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, Initialization) {
    
    // Dimensions don't match: Columns don't match
    EXPECT_THROW(({
        try {
            Matrix<float, 2, 1> m2({{1.f}, {2.f}, {3.f}});
        } catch (std::invalid_argument& ex) {
            EXPECT_STREQ("Incompatible dimensions: Matrix dimensions are [2,1] "
                         "Number of columns in the input is 3", ex.what()); 
            throw;
        }
    }), std::invalid_argument);

    // Dimensions don't match: Rows don't match
    EXPECT_THROW(({
        try {
            Matrix<float, 1, 2> m2({{1.f,2.f}, {2.f}});
        } catch (std::invalid_argument& ex) {
            EXPECT_STREQ("Incompatible dimensions: Matrix dimensions are [1,2] "
                         "Number of rows in column 1 is 2", ex.what()); 
            throw;
        }
    }), std::invalid_argument);
    
    // Assert column major matrix is filled correctly 
    Matrix<int, 2, 2> m3 {{1,2},{3,4}};
    int const* data = m3.getData();
    ASSERT_EQ(data[0], 1);
    ASSERT_EQ(data[1], 2);
    ASSERT_EQ(data[2], 3);
    ASSERT_EQ(data[3], 4);
}

TEST(Matrix, CopyAssignment) {
    Matrix<unsigned, 1, 1> m1 {{45U}};
    Matrix<unsigned, 1, 1> m2(m1);
    
    ASSERT_EQ(m2.getData()[0], 45U);
}

TEST(Matrix, MoveConstruction) {
    Matrix<int, 1, 1> m1 {{10}};
    auto p1 = m1.getData();
    Matrix<int, 1, 1> m2(std::move(m1));
    auto p2 = m2.getData();
    ASSERT_EQ(p1, p2);
    ASSERT_EQ(*p1, *p2);
    ASSERT_EQ(*p1, 10);
    ASSERT_EQ(m1.getData(), nullptr);
    Matrix<int, 1, 1> m3 {Matrix<int, 1, 1>{{25}}};
    ASSERT_EQ(m3.getData()[0], 25);
}

TEST(Matrix, MoveAssignment) {
    Matrix<int, 1, 1> m1 = Matrix<int,1,1>{{10}};
    auto p1 = m1.getData();
    ASSERT_EQ(*p1, 10);
}

TEST(Matrix, Print) {
    Matrix<int, 3, 2> m1 ({ {10, 11, 12}, {10, 11, 12} });
    ofstream ofs("mat.out");   
    ofs << m1;
    ifstream ifs("mat.out");
    string str;
    bool contentsMatch = false;
    getline(ifs,str);
    regex collapseSpaces (R"(\s+)");
    regex trimLeading (R"(^\s)");
    str = regex_replace(str, collapseSpaces, " ");
    str = regex_replace(str, trimLeading, "");
    contentsMatch = str.substr(0, 2) == "10";
    EXPECT_TRUE(contentsMatch) << "Expecting 10 at 0, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "10";
    EXPECT_TRUE(contentsMatch) << "Expecting 10 at 0, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    str = regex_replace(str, collapseSpaces, " ");
    str = regex_replace(str, trimLeading, "");
    contentsMatch = str.substr(0, 2) == "11";
    EXPECT_TRUE(contentsMatch) << "Expecting 11 at 1, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "11";
    EXPECT_TRUE(contentsMatch) << "Expecting 11 at 1, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    str = regex_replace(str, collapseSpaces, " ");
    str = regex_replace(str, trimLeading, "");
    contentsMatch = str.substr(0, 2) == "12";
    EXPECT_TRUE(contentsMatch) << "Expecting 12 at 2, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "12";
    EXPECT_TRUE(contentsMatch) << "Expecting 12 at 2, 1. Found " << str.substr(3) << endl;
}

TEST(Matrix, RowMajor) {

    Matrix<int, 3, 2> m1 ({ {10, 12}, {13, 14}, {15, 16} }, Matrix<int, 3, 2>::Order::RowMajor);
    const int* data = m1.getData();
    EXPECT_EQ(data[0], 10);
    EXPECT_EQ(data[1], 13);
    EXPECT_EQ(data[2], 15);

    EXPECT_EQ(data[3], 12);
    EXPECT_EQ(data[4], 14);
    EXPECT_EQ(data[5], 16);
    
    // Make sure column major internal format is formatted in row major order when printed 
    ofstream ofs("mat1.out");   
    ofs << m1;
    ifstream ifs("mat1.out");
    string str;
    bool contentsMatch = false;
    regex collapseSpaces (R"(\s+)");
    regex trimLeading (R"(^\s)");
    getline(ifs,str);
    str = regex_replace(str, collapseSpaces, " ");
    str = regex_replace(str, trimLeading, "");
    contentsMatch = str.substr(0, 2) == "10";
    EXPECT_TRUE(contentsMatch) << "Expecting 10 at 0, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "12";
    EXPECT_TRUE(contentsMatch) << "Expecting 12 at 0, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    str = regex_replace(str, collapseSpaces, " ");
    str = regex_replace(str, trimLeading, "");
    contentsMatch = str.substr(0, 2) == "13";
    EXPECT_TRUE(contentsMatch) << "Expecting 13 at 1, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "14";
    EXPECT_TRUE(contentsMatch) << "Expecting 14 at 1, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    str = regex_replace(str, collapseSpaces, " ");
    str = regex_replace(str, trimLeading, "");
    contentsMatch = str.substr(0, 2) == "15";
    EXPECT_TRUE(contentsMatch) << "Expecting 15 at 2, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "16";
    EXPECT_TRUE(contentsMatch) << "Expecting 16 at 2, 1. Found " << str.substr(3) << endl;
}

TEST(Matrix, VectorMultiplicationScaling) {
    Vector4D<float> v {10, 10, 10, 1};
    Matrix<float, 4, 4> scalingMatrix {{2, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 2, 0}, {0, 0, 0, 1}};
    Vector4D<float> result = scalingMatrix * v;
    ASSERT_FLOAT_EQ(result.x, 20.f) << "Multiplication results are wrong. Incorrect x coordinate";
    ASSERT_FLOAT_EQ(result.y, 20.f) << "Multiplication results are wrong. Incorrect y coordinate";
    ASSERT_FLOAT_EQ(result.z, 20.f) << "Multiplication results are wrong. Incorrect z coordinate";
    ASSERT_FLOAT_EQ(result.w, 1.f) << "Multiplication results are wrong. Incorrect w coordinate";
}

TEST(Matrix, VectorMultiplicationTranslation) {
    Vector4D<float> v {10, 10, 10, 1};
    Matrix<float, 4, 4> translationMatrix {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {10, 10, 10, 1}};
    Vector4D<float> result = translationMatrix * v;
    ASSERT_FLOAT_EQ(result.x, 20.f) << "Multiplication results are wrong. Incorrect x coordinate";
    ASSERT_FLOAT_EQ(result.y, 20.f) << "Multiplication results are wrong. Incorrect y coordinate";
    ASSERT_FLOAT_EQ(result.z, 20.f) << "Multiplication results are wrong. Incorrect z coordinate";
    ASSERT_FLOAT_EQ(result.w, 1.f) << "Multiplication results are wrong. Incorrect w coordinate";
}

TEST(Matrix, OrthographicProjectionMatrixWithInvalidBounds) {
    EXPECT_THROW({
         try {
             [[maybe_unused]]
             auto orthoMatrix = OrthographicProjectionMatrix<float>({{0,0}, {0,0}, {0,0}});
         } catch (std::runtime_error &ex) {
             throw;
         }
    }, std::runtime_error) << "Expected a runtime error when invalid bounds are specified";
}

TEST(Matrix, OrthographicProjectionMatrix) {
    OrthographicProjectionMatrix<float> testMatrix({{-10,-20,-30}, {10,20, 30}});
    Vector4D<float> vertex { -10, -20, -30, 1 };
    Vector4D<float> result = testMatrix * vertex;
    ASSERT_FLOAT_EQ(-1, result.x) << "x-coordinate is incorrect after projection";
    ASSERT_FLOAT_EQ(-1, result.y) << "x-coordinate is incorrect after projection";
    ASSERT_FLOAT_EQ(+1, result.z) << "x-coordinate is incorrect after projection";
    ASSERT_FLOAT_EQ(+1, result.w) << "x-coordinate is incorrect after projection";
}

TEST(Matrix, ConversionToPointer) {
    IdentityMatrix<float, 3, 3> identityMatrix;
    float const* matrixData = identityMatrix;
    ASSERT_FLOAT_EQ(matrixData[0], 1.);
    ASSERT_FLOAT_EQ(matrixData[4], 1.);
    ASSERT_FLOAT_EQ(matrixData[8], 1.);
    for (auto i : {1,2,3,5,6,7}) {
        ASSERT_FLOAT_EQ(matrixData[i], 0.);
    }
}

TEST(Matrix, ColumnExtraction) {
    Matrix<float, 3, 3> matrix { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    auto column0 = matrix[0];
    ASSERT_FLOAT_EQ(column0[0], 1);
    ASSERT_FLOAT_EQ(column0[1], 2);
    ASSERT_FLOAT_EQ(column0[2], 3);

    auto column1 = matrix[1];
    ASSERT_FLOAT_EQ(column1[0], 4);
    ASSERT_FLOAT_EQ(column1[1], 5);
    ASSERT_FLOAT_EQ(column1[2], 6);

    auto column2 = matrix[2];
    ASSERT_FLOAT_EQ(column2[0], 7);
    ASSERT_FLOAT_EQ(column2[1], 8);
    ASSERT_FLOAT_EQ(column2[2], 9);
}

TEST(Matrix, RowExtraction) {
    Matrix<float, 3, 3> matrix { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    auto row0 = matrix(0);
    ASSERT_FLOAT_EQ(row0[0], 1);
    ASSERT_FLOAT_EQ(row0[1], 4);
    ASSERT_FLOAT_EQ(row0[2], 7);

    auto row1 = matrix(1);
    ASSERT_FLOAT_EQ(row1[0], 2);
    ASSERT_FLOAT_EQ(row1[1], 5);
    ASSERT_FLOAT_EQ(row1[2], 8);

    auto row2 = matrix(2);
    ASSERT_FLOAT_EQ(row2[0], 3);
    ASSERT_FLOAT_EQ(row2[1], 6);
    ASSERT_FLOAT_EQ(row2[2], 9);
}

TEST(Matrix, Transpose) {
    // Inner init list is a column since the default order is column major
    Matrix<float, 3, 3> matrix {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9} };
    auto transposedMatrix = matrix.transpose();
    auto column0 = transposedMatrix[0];
    ASSERT_FLOAT_EQ(column0[0], 1);
    ASSERT_FLOAT_EQ(column0[1], 2);
    ASSERT_FLOAT_EQ(column0[2], 3);

    auto column1 = transposedMatrix[1];
    ASSERT_FLOAT_EQ(column1[0], 4);
    ASSERT_FLOAT_EQ(column1[1], 5);
    ASSERT_FLOAT_EQ(column1[2], 6);

    auto column2 = transposedMatrix[2];
    ASSERT_FLOAT_EQ(column2[0], 7);
    ASSERT_FLOAT_EQ(column2[1], 8);
    ASSERT_FLOAT_EQ(column2[2], 9);
}
