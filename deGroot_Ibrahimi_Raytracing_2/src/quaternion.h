#pragma once

#include "triple.h"

class Quaternion {
public:
    double s;
    Vector v;

    Quaternion();

    Quaternion(double a, double b, double c, double d);

    Quaternion(double s, Vector v);

    Quaternion operator*(const Quaternion& o);

    Quaternion operator/(double d);

    Quaternion conjugate();

    Quaternion inverse();

    void rotate(double angle_deg, const Vector& axis);
};


