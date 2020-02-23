#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

class Triangle : public Object {
public:
    Triangle(Point const& v0,
             Point const& v1,
             Point const& v2);

    virtual Hit intersect(Ray const& ray);

    double area(const Point& a, const Point& b, const Point& c);

    Point v0;
    Point v1;
    Point v2;

    Vector N;
};

#endif
