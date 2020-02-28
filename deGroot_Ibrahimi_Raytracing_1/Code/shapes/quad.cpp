#include "quad.h"
#include "triangle.h"

#include <cmath>

using namespace std;

Hit Quad::intersect(Ray const& ray) {
    Triangle t0(vv0, vv1, vv2);
    Triangle t1(vv0, vv2, vv3);

    Hit t0_hit = t0.intersect(ray);
    Hit t1_hit = t1.intersect(ray);

    if (isnan(t0_hit.t)) return t1_hit;
    return t0_hit;
    if (t0_hit.t && isnan(t1_hit.t)) {
        return t0_hit;
    }
    if (!t0_hit.t && t1_hit.t) {
        return t1_hit;
    }

    // t0_hit.t &&

    if (t0_hit.t < t1_hit.t) {
        return t0_hit;
    } else {
        return t1_hit;
    }
    return Hit::NO_HIT();
}

Quad::Quad(Point const& v0,
           Point const& v1,
           Point const& v2,
           Point const& v3) : vv0(v0), vv1(v1), vv2(v2), vv3(v3) {

}
