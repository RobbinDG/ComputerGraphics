#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>
#include <QOpenGLDebugLogger>

struct Material
{
    QVector3D color;
    GLfloat ka;         // Ambient intensity
    GLfloat kd;         // Diffuse intensity
    GLfloat ks;         // Specular intensity
    GLint p;

    Material(QVector3D color, GLfloat ka, GLfloat kd, GLfloat ks, GLint p)
        :
          color(color),
          ka(ka),
          kd(kd),
          ks(ks),
          p(p)
    {}
};

#endif // MATERIAL_H

