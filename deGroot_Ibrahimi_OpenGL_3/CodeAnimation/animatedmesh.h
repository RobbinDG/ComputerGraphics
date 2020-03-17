#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include "mesh.h"
#include "animation.h"
#include "drawable.h"

/**
 * @brief The AnimatedMesh class
 * A class containting a drawable object and an animation to apply to it.
 */
class AnimatedMesh : public Drawable
{
private:
    Drawable* mesh;
    Animation* animation;

public:
    AnimatedMesh(Drawable* mesh, Animation* animation);
    ~AnimatedMesh();
    void setTranslation(const QVector3D& t) override;
    void translate(const QVector3D& t) override;
    void setRotation(const QVector3D& r) override;
    void setScale(float s) override;
    void setTransform(const QVector3D& t, const QVector3D& r, float s) override;
    void resetTransform() override;
    void transform(const QVector3D& t, const QVector3D& r, float s) override;
    void transform(const QMatrix4x4& m) override;
    void setShadingMode(ShadingMode shading) override;
    void draw(QVector3D& lightPosition, const QVector3D& lightColor, const QMatrix4x4& projectionTransform) override;

};

#endif // ANIMATEDMESH_H
