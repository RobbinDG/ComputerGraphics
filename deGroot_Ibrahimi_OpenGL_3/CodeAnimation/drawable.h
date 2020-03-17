#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

class Drawable
{
public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    Drawable();
    virtual ~Drawable();
    virtual void setTranslation(const QVector3D& t) = 0;
    virtual void translate(const QVector3D& t) = 0;
    virtual void setRotation(const QVector3D& r) = 0;
    virtual void setScale(float s) = 0;
    virtual void setTransform(const QVector3D& t, const QVector3D& r, float s) = 0;
    virtual void resetTransform() = 0;
    virtual void transform(const QVector3D& t, const QVector3D& r, float s) = 0;
    virtual void setShadingMode(ShadingMode shading) = 0;
    virtual void draw(QVector3D& lightPosition, const QVector3D& lightColor, const QMatrix4x4& projectionTransform) = 0;
};

#endif // DRAWABLE_H
