#include "quad.h"
#include "triangle.h"

#include <limits>

using namespace std;

Hit Quad::intersect(Ray const& ray) {
    double max_hit = numeric_limits<double>::infinity();
    Hit t0_hit = t0.intersect(ray);
    Hit t1_hit = t1.intersect(ray);
    if (t0_hit.t < max_hit) return t0_hit;
    if (t1_hit.t < max_hit) return t1_hit;
    return Hit::NO_HIT();
}

Quad::Quad(Point const& v0,
           Point const& v1,
           Point const& v2,
           Point const& v3) : t0({v0, v2, v1}), t1({v0, v2, v3}) {}
