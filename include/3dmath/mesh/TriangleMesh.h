#pragma once
#include <cstdint>
#include "3dmath/Vector.h"
#include "3dmath/TypeAliases.h"

namespace math3d {
    class TriangleMesh {
    public:
        TriangleMesh() = default;

        [[nodiscard]] uint64_t getNumberOfVertices() const {
            return vertices.size();
        }

        [[nodiscard]] uint64_t getNumberOfFaces() const {
            return faces.size();
        }

        void allocate(uint64_t const numVertices, uint64_t const numFaces) {
            vertices.reserve(numVertices);
            faces.reserve(numFaces);
        }

        void addVertex(types::Vertex vertex) {
            vertices.push_back(std::move(vertex));
        }

        void addFace(types::Tri tri) {
            faces.push_back(std::move(tri));
        }

        [[nodiscard]]
        types::Vertex getVertex(uint64_t const vertexIndex) const {
            return vertices.at(vertexIndex);
        }

        [[nodiscard]]
        types::Tri getFace(uint64_t const faceIndex) const {
            return faces.at(faceIndex);
        }

        [[nodiscard]]
        types::Vertex const* getVertices() const {
            return vertices.data();
        }

        [[nodiscard]]
        types::Tri const* getFaces() const {
            return faces.data();
        }

    private:
        types::Vertices vertices;
        types::Tris faces;
    };
}