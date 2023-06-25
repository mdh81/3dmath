#pragma once

#include "Constants.h"
#include "Utilities.h"
#include "Vector.h"
#include <cmath>
#include <ostream>

namespace math3d {

    class PolarCoordinates {

    public:
        PolarCoordinates(float radius, float theta)
        : radius(radius)
        , theta(theta) {
        }

        [[nodiscard]]
        virtual Vector3D<float> getCartesianCoordinates() const {
            return { radius * cos(theta), radius * sin(theta), 0.f };
        }

        [[nodiscard]]
        static PolarCoordinates convertCartesianCoordinates(Vector2D<float> const& point) {
            if (point.x < constants::tolerance && point.y < constants::tolerance) {
                // Prevent UB in atan2 when y and x are zeroes
                return {0.f, 0.f};
            } else {
                // NOTE: Instead of atan2, we can use acos(x/r) or asin(y/r) to compute theta
                return {sqrt((point.x * point.x) + (point.y * point.y)), atan2(point.y, point.x)};
            }
        }

        [[nodiscard]]
        float getRadius() const {
            return radius;
        }

        [[nodiscard]]
        float getTheta() const {
            return theta;
        }

    protected:
        float theta;
        float radius;
    };

    // Describes a point P in space using the spherical coordinates (r, theta, phi)
    class SphericalCoordinates : public PolarCoordinates {

    public:
        SphericalCoordinates(float radius, float theta, float phi)
        : PolarCoordinates(radius, theta)
        , phi(phi) {
        }


        // See https://github.com/mdh81/3dmath#conversion-of-spherical-coordinates-to-cartesian-coordinates
        // for a derivation of this formula
        [[nodiscard]]
        Vector3D<float> getCartesianCoordinates() const override {
            return { radius * sin(phi) * cos(theta),
                     radius * sin(phi) * sin(theta),
                     radius * cos(phi)};
        }

    private:
        float phi;
    friend std::ostream& operator<<(std::ostream&, SphericalCoordinates const&);
    };


inline std::ostream& operator<<(std::ostream& os, SphericalCoordinates const& sphericalCoordinates) {
    os << "Radius = " << sphericalCoordinates.radius << ", Theta = " << Utilities::asDegrees(sphericalCoordinates.theta) << ", Phi = " << Utilities::asDegrees(sphericalCoordinates.phi);
    return os;
}

}