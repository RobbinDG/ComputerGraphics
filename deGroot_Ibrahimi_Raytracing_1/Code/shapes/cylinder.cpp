#include "cylinder.h"

#include <cmath>

using namespace std;

Hit Cylinder::intersect(Ray const& ray) {
    Vector S;
    if (direction.x > 0) S = Vector(0, 1, 1);
    else if (direction.y > 0) S = Vector(1, 0, 1);
    else S = Vector(1, 1, 0);

    double a = S.dot(ray.D * ray.D);
    double b = 2 * S.dot((ray.O - position) * ray.D);
    double c = S.dot((ray.O - position) * (ray.O - position)) - radius * radius;
    double det = b * b - 4 * a * c;

    double t;
    if (det > 0.0) {
        double t1, t2;
        t1 = (-b + sqrt(det))/ 2 * a;
        t2 = (-b - sqrt(det))/ 2 * a;

        if (t1 < 0.0 && t2 < 0.0) return Hit::NO_HIT();
        if (t1 < 0.0) t = t2;
        else if (t2 < 0.0) t = t1;
        else t = min(t1, t2);
    } else if (det == 0.0) {
        t = (-b + sqrt(det))/ 2 * a;
    } else {
        return Hit::NO_HIT();
    }

    Point X = ray.at(t);
    Vector N = (X - position).normalized();
    return {t, N};
}

Cylinder::Cylinder(Point const& pos, Vector const& direction, double radius)
        :
        position(pos),
        direction(direction),
        radius(radius) {}
