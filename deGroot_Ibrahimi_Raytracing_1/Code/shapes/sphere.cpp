#include "sphere.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Sphere::intersect(Ray const& ray) {
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (position) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // Placeholder for actual intersection calculation.
    /*
    Vector originToPosition = (position - ray.O).normalized();
    if (originToPosition.dot(ray.D) < 0.999)
        return Hit::NO_HIT();
*/
    double a, b, c, disc, t;
    a = ray.D.dot(ray.D);
    b = 2 * (ray.O - position).dot(ray.D);
    c = (ray.O - position).dot(ray.O - position) - r * r;
    disc = b * b - 4 * a * c;

    if (disc > 0.0) { // 2 solutions, take the closest
        double p,q;
        p = (-b + sqrt(disc)) / (2 * a); q = (-b - sqrt(disc)) / (2 * a);
        if (p < 0 && q < 0) return Hit::NO_HIT();
        if (p < 0) t = q;
        else if (q < 0) t = p;
        else t = min(p, q);
    } else if (disc == 0.0) { // 1 solution, calculate either
        t = (-b - sqrt(disc)) / (2 * a);
    } else {
        return Hit::NO_HIT();
    }

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, position, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/
    Vector N /* = ... */;

    return t < 0.0 ? Hit::NO_HIT() : Hit(t, N);
}

Sphere::Sphere(Point const& pos, double radius)
        :
        position(pos),
        r(radius) {}