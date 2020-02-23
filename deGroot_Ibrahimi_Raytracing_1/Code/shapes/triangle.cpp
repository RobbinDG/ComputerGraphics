#include <iostream>
#include "triangle.h"
#include <cmath>

Hit Triangle::intersect(Ray const& ray) {
    double epsilon = 0.00001;

    // Check for ray parallel to triangle
    if (epsilon > ray.D.dot(N) && -epsilon < ray.D.dot(N)) return Hit::NO_HIT();

    // Calculate t
    double t = (N.dot(v0) - N.dot(ray.O)) / (N.dot(ray.D));
    Point X = ray.at(t);

    // Calculate alpha and beta such that X = P + alpha * R + beta * Q
    Point P = v0, R = v2 - v0, Q = v1 - v0;
    if (fabs(Q.x * R.y - R.x * Q.y) < epsilon || fabs(Q.x) < epsilon) return Hit::NO_HIT();

    double beta = (Q.x * (X.y - P.y) + Q.y * (P.x - X.x)) / (Q.x * R.y - R.x * Q.y);
    double alpha = (X.x - P.x - beta * R.x) / Q.x;

    if (alpha < 0.0 || beta < 0.0 || alpha + beta > 1.0 + 0.0) return Hit::NO_HIT();

    return t >= 0 ? Hit(t, N) : Hit::NO_HIT();
}

Triangle::Triangle(Point const& v0,
                   Point const& v1,
                   Point const& v2)
        :
        v0(v0),
        v1(v1),
        v2(v2),
        N((v1 - v0).cross(v2 - v0).normalized()) {
    // Calculate the surface normal here and store it in the N,
    // which is declared in the header. It can then be used in the intersect function.
}

double Triangle::area(const Point& a, const Point& b, const Point& c) {
    return fabs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2);
}
