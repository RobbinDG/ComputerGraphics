#include "quad.h"
#include "triangle.h"

#include <limits>
#include <cmath>

using namespace std;

Hit Quad::intersect(Ray const& ray) {
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    Hit t0_hit = t0.intersect(ray);
    Hit t1_hit = t1.intersect(ray);

    // Take the min of both t0_hit and t1_hit
    min_hit = t0_hit.t < min_hit.t ? t0_hit : min_hit;
    min_hit = t1_hit.t < min_hit.t ? t1_hit : min_hit;

    // If any of them are hit, we return min_hit. Else, there was no hit
    if (t0_hit.t < numeric_limits<double>::infinity() || t1_hit.t < numeric_limits<double>::infinity()) {
        return min_hit;
    }
    return Hit::NO_HIT();
}

Quad::Quad(Point const& v0,
           Point const& v1,
           Point const& v2,
           Point const& v3) : t0({v0, v0, v0}), t1({v0, v2, v3}) {

    double maxLength = 0;

    // Get the max length and update the t0 and t1 according to the vector with the biggest length
    if (fabs((v0 - v1).length()) > maxLength) {
        t0 = Triangle(v0, v1, v2);
        t1 = Triangle(v0, v1, v3);
        maxLength = fabs((v0 - v1).length());
    }

    if (fabs((v0 - v2).length()) > maxLength) {
        t0 = Triangle(v0, v2, v1);
        t1 = Triangle(v0, v2, v3);
        maxLength = fabs((v0 - v2).length());
    }

    if (fabs((v0 - v3).length()) > maxLength) {
        t0 = Triangle(v0, v3, v1);
        t1 = Triangle(v0, v3, v2);
        maxLength = fabs((v0 - v3).length());
    }

    if (fabs((v1 - v2).length()) > maxLength) {
        t0 = Triangle(v1, v2, v0);
        t1 = Triangle(v1, v2, v3);
        maxLength = fabs((v1 - v2).length());
    }

    if (fabs((v1 - v3).length()) > maxLength) {
        t0 = Triangle(v1, v3, v0);
        t1 = Triangle(v1, v3, v2);
        maxLength = fabs((v1 - v3).length());
    }

    if (fabs((v2 - v3).length()) > maxLength) {
        t0 = Triangle(v2, v3, v0);
        t1 = Triangle(v2, v3, v1);
    }
}
