#ifndef MATH3D_LINE_H
#define MATH3D_LINE_H

#include "Vector.h"
#include <string>
#include <sstream>

namespace math3d {

    template<typename T>
    class Line2D {

        using Vector2D = math3d::Vector2D<T>;

        public:
            // Parameteric form
            Line2D(Vector2D const& ptOnLine, Vector2D const& direction) {

            }

            // Implict form
            Line2D(T const a, T const b, T const c) {
                m_a = a;
                m_b = b;
                m_c = c;
                computeParametricForm();
            }

            Vector2D getDirection() const {
                return m_direction;
            }

            Vector2D getNormal() const {
                // Return a direction that's perpendicular to the line's
                // direction. We can choose (-d2, d1) or (d1, -d2)
                return {-m_direction.y, m_direction.x}; 
            }

            Vector2D getPointOnTheLine() const {
                return m_ptOnLine;
            }

            Vector2D getE1Intercept() const {
                return {fabs(m_a) > 1e-6 ? -m_c/m_a : nan, 0};
            }

            Vector2D getE2Intercept() const {
                return {0, fabs(m_b) > 1e-6 ? -m_c/m_b : nan};
            }

            std::string getImplicitForm() const {
                std::stringstream ss;
                ss << m_a << "x" << (m_b > 0 ? "+" : "") << m_b << "y" << m_c << "=0";  
                return ss.str();
            }

            std::string getParametericForm() const {
                std::stringstream ss;
                ss << std::setprecision(3);
                ss << m_ptOnLine; 
                ss << "+t";
                ss << m_direction;
                return ss.str();
            }

        private:
            void computeParametricForm() {
                // Direction vector of the line will be perpendicular to 
                // the normal. Given a direction (d1, d2), it's perpendicular
                // can be either (-d2, d1) or (d2, -d1), we choose the former
                m_direction.x = -m_b;
                m_direction.y =  m_a;

                // Choose x or y intercept as the point on the line
                // Page 39 in Geometry Toolbox suggests using the intercept
                // that is closest to the origin to improve the "numerical
                // stability" of calculations. It makes an interesting observation
                // that if ||a|| > ||b|| then x intercept is closer, otherwise y
                // is closer
                // NOTE: Lines that are parallel to either x or y axis are implicitly
                // handled by the comparision, so there is no need to check for division
                // by zero to determine that the line is parallel to x or y
                if (fabs(m_a) > fabs(m_b)) {
                    m_ptOnLine.x = -m_c/m_a;
                    m_ptOnLine.y = 0;
                } else {
                    m_ptOnLine.x = 0;
                    m_ptOnLine.y = -m_c/m_b;
                } 
            }

            void computeImplicitForm() {

            }

        private:
            Vector2D m_ptOnLine;
            Vector2D m_direction;
            T m_a;
            T m_b;
            T m_c;
    };
}

#endif