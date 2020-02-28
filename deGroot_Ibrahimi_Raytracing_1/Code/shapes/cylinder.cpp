#include "cylinder.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Cylinder::intersect(Ray const& ray) {
    // Map direction axis to a vector
    Vector S;
    if (direction.x > 0) S = Vector(0, 1, 1);
    else if (direction.y > 0) S = Vector(1, 0, 1);
    else S = Vector(1, 1, 0);

    // Hits cylinder either on side or top/bottom, choose the closest.
    Hit hit_side = intersect_side(ray, S);
    Hit hit_tops = intersect_tops(ray, S);

    if (isnan(hit_side.t)) return hit_tops;
    return hit_side;
}

Cylinder::Cylinder(Point const& pos, Vector const& direction, double radius)
        :
        position(pos),
        direction(direction),
        radius(radius) {}

Hit Cylinder::intersect_side(Ray const& ray, const Vector& S) {
    // Calculate t
    double a = S.dot(ray.D * ray.D);
    double b = S.dot(2 * (ray.O - position) * ray.D);
    double c = S.dot((ray.O - position) * (ray.O - position)) - radius * radius;
    double det = b * b - 4 * a * c;

    double t;
    if (det > 0.0) {
        double t1, t2;
        t1 = (-b + sqrt(det)) / (2 * a);
        t2 = (-b - sqrt(det)) / (2 * a);

        if (t1 < 0.0 && t2 < 0.0) return Hit::NO_HIT();
        if (t1 < 0.0) t = t2;
        else if (t2 < 0.0) t = t1;
        else t = min(t1, t2);
    } else if (det == 0.0) {
        t = -b / (2 * a);
    } else {
        // No hit on side of cylinder
        return Hit::NO_HIT();
    }

    // Calculate intersection in case of hit on side of cylinder
    Point X = ray.at(t);

    double h = (X - position).dot(direction);
    if (h > direction.dot(direction) || h < 0.0) return Hit::NO_HIT();

    Vector N = (X - (position * S + X * (Vector(1, 1, 1) - S))).normalized();

    return {t, N};
}

Hit Cylinder::intersect_tops(Ray const& ray, const Vector& S) {
    // Calculate normal, is always in a unit direction
    Vector N = Vector(1, 1, 1) - S;

    // Find t with intersection of 'bottom' side
    double t = N.dot(position - ray.O) / (N.dot(ray.D));
    if (t < 0.0) return Hit::NO_HIT();
    Point X = ray.at(t);
    if (((X - position) * (X - position)).dot(S) <= radius * radius)
        return {t, N}; // Normal is flipped in scene.cpp if not visible

    // Find t with intersection of 'top' side
    t = ((position + direction - ray.O).dot(N)) / (N.dot(ray.D));
    if (t < 0.0) return Hit::NO_HIT();
    X = ray.at(t);
    if (((X - position) * (X - position)).dot(S) <= radius * radius)
        return {t, N}; // Normal is flipped in scene.cpp if not visible

    return Hit::NO_HIT();
}
