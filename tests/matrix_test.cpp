#include "gtest/gtest.h"
#include "3dmath/Matrix.h"
#include <vector>
#include <initializer_list>
#include <iostream>
#include <fstream>
using namespace std;
using namespace math3d;

TEST(Matrix, TestDefaultConstruction) {
    // Assert all matrix entries are zeroes
    Matrix<float, 10, 10> m; 
    ASSERT_EQ(m.getNumberOfRows(), 10);
    ASSERT_EQ(m.getNumberOfColumns(), 10);
    float const* data = m.getData();
    for (int i = 0; i < 99; ++i) {
        ASSERT_FLOAT_EQ(0, data[i]);
    }
}

TEST(Matrix, TestIdentity) {
    
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

TEST(Matrix, TestCopyConstruction) {
    
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

TEST(Matrix, TestInitialization) {
    
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

TEST(Matrix, TestCopyAssignment) {
    Matrix<unsigned, 1, 1> m1 {{45U}};
    Matrix<unsigned, 1, 1> m2(m1);
    
    ASSERT_EQ(m2.getData()[0], 45U);
}

TEST(Matrix, TestMoveConstruction) {
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

TEST(Matrix, TestMoveAssignment) {
    Matrix<int, 1, 1> m1 = Matrix<int,1,1>{{10}};
    auto p1 = m1.getData();
    ASSERT_EQ(*p1, 10);
}

TEST(Matrix, TestPrint) {
    Matrix<int, 3, 2> m1 ({ {10, 11, 12}, {10, 11, 12} });
    ofstream ofs("mat.out");   
    ofs << m1;
    ifstream ifs("mat.out");
    string str;
    bool contentsMatch = false;
    getline(ifs,str);
    contentsMatch = str.substr(0, 2) == "10";
    EXPECT_TRUE(contentsMatch) << "Expecting 10 at 0, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "10";
    EXPECT_TRUE(contentsMatch) << "Expecting 10 at 0, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    contentsMatch = str.substr(0, 2) == "11";
    EXPECT_TRUE(contentsMatch) << "Expecting 11 at 1, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "11";
    EXPECT_TRUE(contentsMatch) << "Expecting 11 at 1, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    contentsMatch = str.substr(0, 2) == "12";
    EXPECT_TRUE(contentsMatch) << "Expecting 12 at 2, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "12";
    EXPECT_TRUE(contentsMatch) << "Expecting 12 at 2, 1. Found " << str.substr(3) << endl;
}

TEST(Matrix, TestRowMajor) {

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
    getline(ifs,str);
    contentsMatch = str.substr(0, 2) == "10";
    EXPECT_TRUE(contentsMatch) << "Expecting 10 at 0, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "12";
    EXPECT_TRUE(contentsMatch) << "Expecting 12 at 0, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    contentsMatch = str.substr(0, 2) == "13";
    EXPECT_TRUE(contentsMatch) << "Expecting 13 at 1, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "14";
    EXPECT_TRUE(contentsMatch) << "Expecting 14 at 1, 1. Found " << str.substr(3) << endl;
    getline(ifs, str);
    contentsMatch = str.substr(0, 2) == "15";
    EXPECT_TRUE(contentsMatch) << "Expecting 15 at 2, 0. Found " << str.substr(0,2) << endl;
    contentsMatch = str.substr(3) == "16";
    EXPECT_TRUE(contentsMatch) << "Expecting 16 at 2, 1. Found " << str.substr(3) << endl;
}
