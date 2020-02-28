#include "triangle.h"
#include <cmath>
#include <iostream>

Hit Triangle::intersect(Ray const& ray) {
    double epsilon = 0.00001;

    // Check for ray parallel to triangle
    if (epsilon > fabs(ray.D.dot(N))) return Hit::NO_HIT();
    double f = ray.D.dot(N);

    // Calculate t
    double t = (N.dot(v0 - ray.O)) / (N.dot(ray.D));
    if (t < 0) return Hit::NO_HIT();

    // Find intersecting point
    Point X = ray.at(t);

    // do a left-right test on each edge
    if ((v1 - v0).cross(X - v0).dot(N) < 0) return Hit::NO_HIT();
    if ((v2 - v1).cross(X - v1).dot(N) < 0) return Hit::NO_HIT();
    if ((v0 - v2).cross(X - v2).dot(N) < 0) return Hit::NO_HIT();

    return {t, N};
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
