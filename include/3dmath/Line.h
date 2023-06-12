#ifndef MATH3D_LINE_H
#define MATH3D_LINE_H

#include "Vector.h"
#include <string>
#include <sstream>
#include <limits>
#include <sstream>

namespace math3d {
    // TODO: Make Line a primitive
    template<typename T>
    using Point2D = Vector2D<T>;

    template<typename T>
    class Line2D {

        public:
            virtual std::string toString() const = 0;
            virtual Vector2D<T> getNormal() const = 0;
            virtual Vector2D<T> getDirection() const = 0;
            virtual Point2D<T> getPointOnTheLine() const = 0;
            virtual Point2D<T> closestPoint(Point2D<T> const&) const = 0;
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
                    ptOnLine.x = 0;
                    ptOnLine.y = -m_c/m_b;
                }
                return ParametricLine2D(ptOnLine, dirVec);
            }

            Vector2D<T> getNormal() const override {
                return Vector2D<T>{m_a, m_b};
            }

            Vector2D<T> getDirection() const override {
                return getParametric().getDirection();
            }

            Point2D<T> getPointOnTheLine() const override {
                if (fabs(m_a) > fabs(m_b)) {
                    return {-m_c/m_a, 0};
                } else {
                    return {0, -m_c/m_b};
                }
            }

            Point2D<T> closestPoint(Point2D<T>const& point) const override {
                return getParametric().closestPoint(point);
            }

            Point2D<T> getXIntercept() const {
                return Point2D<T>{-m_c/m_a,0};
            }

            Point2D<T> getYIntercept() const {
                return Point2D<T>{0,-m_c/m_b};
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
                // Compute a and b as coordinates of the normal vector
                T a, b;
                a = -m_direction.y;
                b = m_direction.x;

                // c can be computed using the dot product between the line direction and its normal
                // If p and q are two points on the line
                // (q - p) . normal = 0
                // |q1-p1| dot |a|  = 0
                // |q2-p2|     |b|
                // a(q1-p1) + b(q2-p2) = 0
                // aq1-ap1+bq2-bp2 = 0
                // aq1+bq2-ap1-bp2 = 0
                // ax + by + c = 0 is the implicit equation of the line
                // where c = -(ap1+bp2)
                // Treating m_point as p, we finally have c as
                T c = -(a*m_point.x + b*m_point.y);
                return ImplicitLine2D<T>(a, b, c);
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

            Vector2D<T> closestPoint(Point2D<T> const& pointInSpace) const override {
                // Let P be m_point, Q be pointInSpace, and v be m_direction. This method will return
                // Q', which is the closest point on the line. Q' is P + tV and is computed by constructing
                // w as the vector from P to Q and considering t as the angle between v and w
                //
                //        Q
                //        /|
                //   w  /  |
                //     / t |
                //   P ->--Q'
                //     v
                //   --t|v|--
                //
                //  v.w = |v||w|cos(t)
                // and cos(t) = v.w
                //              ---
                //            |v||w|
                // From the right triangle PQQ'
                // cos(t) = t|v|
                //          ---
                //          |w|
                // therefore, t|v|   =  v.w
                //            ---       ---
                //            |w|       |v||w|
                // => t = v.w|w|
                //        ---
                //        |v||w||v|
                // t = v.w
                //     ---
                //     |v|^2
                auto w = pointInSpace - m_point;
                T t = m_direction.dot(w) / (m_direction.length() * m_direction.length());
                return m_point + t * m_direction;
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