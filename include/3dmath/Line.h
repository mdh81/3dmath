#ifndef MATH3D_LINE_H
#define MATH3D_LINE_H

#include "Vector.h"
#include <string>
#include <sstream>
#include <limits>
#include <sstream>

namespace math3d {

    template<typename T>
    class Line2D {

        public:
            virtual std::string toString() const = 0;
            virtual Vector2D<T> getNormal() const = 0;
            virtual Vector2D<T> getDirection() const = 0;
            virtual Point2D<T> getPointOnTheLine() const = 0;
            //virtual void distanceToPoint(Point2D<T> const&) const = 0;
    };

    template <typename T>
    class ParametricLine2D;

    template<typename T>
    class ImplicitLine2D : public Line2D<T> {
        public:
            ImplicitLine2D(T const a, T const b, T const c) {
                m_a = a;
                m_b = b;
                m_c = c;
            }

            ImplicitLine2D() = delete;

            ParametricLine2D<T> getParametric() const {
                // Direction vector is the vector perpendicular to the normal
                Vector2D<T> dirVec;
                dirVec.x = -m_b;
                dirVec.y = m_a;

                // Point on the line. Pick x or y-intercept depending on
                // whichever one is greater. Geometry tool box recommends
                // choosing the one with the largest absolute value for
                // numerical stability
                // The formula is computed from the implicit form of the line
                // ax + by + c = 0
                // By definition, Y is 0 at the x-intercept of the line, so
                // ax + c = 0
                // x = -c/a
                // For the same reason,
                // y = -c/b 
                Point2D<T> ptOnLine;
                if (fabs(m_a) > fabs(m_b)) {
                    ptOnLine.x = -m_c/m_a;
                    ptOnLine.y = 0;
                } else {
                    ptOnLine.y = -m_c/m_b;
                    ptOnLine.x = 0;    
                }
                return ParametricLine2D(ptOnLine, dirVec);
            }

            math3d::Vector2D<T> getNormal() const override {
                return Vector2D<T>{m_a, m_b};
            }

            math3d::Vector2D<T> getDirection() const override {
                return getParametric().getDirection();
            }

            math3d::Vector2D<T> getPointOnTheLine() const override {
                return getParametric().getPointOnTheLine();
            }

            std::string toString() const override {
                std::stringstream ss;
                ss << std::setprecision(2);
                ss << m_a << 'x';
                ss << std::showpos;
                ss << m_b << 'y';
                ss << std::showpos << m_c;
                ss << "=0";
                return ss.str();
            }

            private:
                T m_a;
                T m_b;
                T m_c;

    };

    template<typename T>
    class ParametricLine2D : public Line2D<T> {

        public:
            ParametricLine2D(Point2D<T> const& ptOnLine, Vector2D<T> const& direction) {
                m_point = ptOnLine;
                m_direction = direction;
            }

            ParametricLine2D() = delete;

            ImplicitLine2D<T> getImplicit() const {
                // Compute A and B as coordinates of the normal vector
                Vector2D<T> normal;
                normal.x = -m_direction.y;
                normal.y = m_direction.x; 

                // Compute C from X or Y intercept. Using the same
                // recommendation from Geometry toolbox, use the largest
                // absolute value of the intercept for numerical stability
                T c;
                if (fabs(normal.x) > fabs(normal.y)) {
                    c = -(normal.x * m_point.x);
                } else {
                    c = -(normal.y * m_point.y);
                }

                return ImplicitLine2D<T>(normal.x, normal.y, c);
            }

            Vector2D<T> getNormal() const override {
                return getImplicit().getNormal();
            }

            Vector2D<T> getDirection() const override {
                return m_direction;
            }

            Vector2D<T> getPointOnTheLine() const override {
                return m_point;
            }

            std::string toString() const override {
                std::stringstream ss;
                ss << std::setprecision(2);
                ss << m_point;
                ss << "+t";
                ss << m_direction;
                return ss.str();
            }

        private:
            Point2D<T> m_point;
            Vector2D<T> m_direction;
    };
}

#endif