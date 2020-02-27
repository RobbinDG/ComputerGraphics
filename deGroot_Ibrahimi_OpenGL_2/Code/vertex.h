#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

struct Vertex
{
    QVector3D coords;
    QVector3D color;
    QVector3D normal;

    Vertex(QVector3D coords, QVector3D color, QVector3D normal)
        :
          coords(coords),
          color(color),
          normal(normal)
    {}
};

#endif // VERTEX_H

