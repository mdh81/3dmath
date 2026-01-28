#pragma once

#include "Plane.h"

namespace math3d {

    class Triangle final : public Plane {
    public:
        using Point3D = types::Point3D;
        using Points = std::array<Point3D, 3>;

        Triangle(Point3D const& ptA, Point3D const& ptB, Point3D const& ptC)
            : Triangle(std::array{ptA, ptB, ptC}) {
        }

        explicit Triangle(Points const& vertices)
            : Plane(
                vertices[0],
                (vertices[1] - vertices[0]) * (vertices[2] - vertices[1]))
            , vertices(vertices) {
        }

        Points const& getPoints() const {
            return vertices;
        }

        [[nodiscard]]
        double getArea() const {
            auto const ab = vertices[1] - vertices[0];
            auto const ac = vertices[2] - vertices[0];
            return 0.5 * (ab * ac).length();
        }

        [[nodiscard]]
        Point3D getBarycentricCoordinates(Point3D const& pointInSpace) const {
            auto area = getArea();
            Triangle a {vertices[0], pointInSpace, vertices[1] };
            Triangle b {vertices[1], pointInSpace, vertices[2] };
            Triangle c {vertices[2], pointInSpace, vertices[0] };
            return { a.getArea() / area, b.getArea() / area, c.getArea() / area };
        }

        [[nodiscard]]
        bool isPointInTriangle(Point3D const& pointInSpace) const {
            auto barycentricCoordinates = getBarycentricCoordinates(pointInSpace);
            return fabs(barycentricCoordinates.x + barycentricCoordinates.y + barycentricCoordinates.z - 1) <
                constants::tolerance;
        }

    private:
        Points vertices;
    };

}