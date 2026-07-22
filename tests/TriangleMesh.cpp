#include "3dmath/mesh/TriangleMesh.h"
#include "gtest/gtest.h"

using namespace std;
using namespace math3d;

TEST(TriangleMesh, Construction) {
    TriangleMesh triMesh;
    ASSERT_TRUE(triMesh.getNumberOfFaces() == 0);
    ASSERT_TRUE(triMesh.getNumberOfVertices() == 0);
}