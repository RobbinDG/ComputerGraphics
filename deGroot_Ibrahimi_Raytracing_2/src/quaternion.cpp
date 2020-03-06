#include <cmath>
#include "quaternion.h"


Quaternion::Quaternion() : s(0.0), v(0.0, 0.0, 0.0) {
}

Quaternion::Quaternion(double a, double b, double c, double d) : s(a), v(b, c, d) {
}

Quaternion::Quaternion(double s, Vector v) : s(s), v(v) {
}

Quaternion Quaternion::operator*(const Quaternion& o) {
    Quaternion r;
    r.s = s * o.s - v.dot(o.v);
    r.v = s * o.v + o.s * v + v.cross(o.v);
    return r;
}

Quaternion Quaternion::conjugate() {
    return {s, -v};
}

Quaternion Quaternion::inverse() {
    return this->conjugate() / (s * s + v.length_2());
}

Quaternion Quaternion::operator/(double d) {
    return {s / d, v / d};
}

void Quaternion::rotate(double angle_deg, const Vector& axis) {
    double alpha = angle_deg * M_PI / 180;
    Quaternion q(cos(alpha / 2), axis * sin(alpha / 2));
    Quaternion q1(cos(alpha / 2), -axis * sin(alpha / 2));
    *this = q * *this * q1;
}
