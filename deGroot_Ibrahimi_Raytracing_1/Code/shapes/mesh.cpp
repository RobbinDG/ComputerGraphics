#include "mesh.h"

#include "../objloader.h"
#include "../vertex.h"
#include "triangle.h"

#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

Hit Mesh::intersect(Ray const& ray) {
    // Replace the return of a NO_HIT by determining the intersection based
    // on the ray and this class's data members.
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (const auto& tri : d_tris) {
        Hit hit(tri->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            obj = tri;
        }
    }
    return !obj ? Hit::NO_HIT() : min_hit;
}

Mesh::Mesh(string const& filename, Point const& position, Vector const& rotation,
           Vector const& scale) {
    OBJLoader model(filename);
    d_tris.reserve(model.numTriangles());
    vector<Vertex> vertices = model.vertex_data();
    for (size_t tri = 0; tri != model.numTriangles(); ++tri) {
        Vertex one = vertices[tri * 3];
        Point v0(one.x, one.y, one.z);

        Vertex two = vertices[tri * 3 + 1];
        Point v1(two.x, two.y, two.z);

        Vertex three = vertices[tri * 3 + 2];
        Point v2(three.x, three.y, three.z);

        // Apply non-uniform scaling, rotation and translation to the three points
        // of the triangle (v0, v1, and v2) here.

        // Non-uniform scaling
        v0 = v0 * scale;
        v1 = v1 * scale;
        v2 = v2 * scale;

        // Rotation
        rotate(v0, rotation);
        rotate(v1, rotation);
        rotate(v2, rotation);

        // Translation
        v0 += position;
        v1 += position;
        v2 += position;

        d_tris.push_back(ObjectPtr(new Triangle(v0, v1, v2)));
    }

    cout << "Loaded model: " << filename << " with " <<
         model.numTriangles() << " triangles.\n";
}

void Mesh::rotate(Point& p, Vector r) {
    auto rotx1 = Vector(1, 0, 0);
    auto rotx2 = Vector(0, cos(r.x), -sin(r.x));
    auto rotx3 = Vector(0, sin(r.x), cos(r.x));
    p = Vector(rotx1.dot(p), rotx2.dot(p), rotx3.dot(p));

    auto roty1 = Vector(cos(r.y), 0, sin(r.y));
    auto roty2 = Vector(0, 1, 0);
    auto roty3 = Vector(-sin(r.y), 0, cos(r.y));
    p = Vector(roty1.dot(p), roty2.dot(p), roty3.dot(p));

    auto rotz1 = Vector(cos(r.z), -sin(r.z), 0);
    auto rotz2 = Vector(sin(r.z), cos(r.z), 0);
    auto rotz3 = Vector(0, 0, 1);
    p = Vector(rotz1.dot(p), rotz2.dot(p), rotz3.dot(p));
}
