#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"

#include <vector>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

/**
 * @brief The Scene class
 * A scene object. Acts as a collection for drawable objects.
 * Enables transformation wrt the scene origin.
 */
class Scene
{
private:
    std::vector<Drawable*> _objects;

    // Phong model constants.
    QVector3D lightPosition = {0.0F, 100.0F, 0.0F};
    QVector3D lightColor = {1.0F, 1.0F, 1.0F};
    QMatrix4x4 projectionTransform;

    // Transforms
    QVector3D translation, rotation;
    float scale = 1.0F;

public:
    Scene();
    ~Scene();

    size_t addObject(Drawable* obj);
    Drawable* getObject(size_t idx);
    const std::vector<Drawable*> meshes();
    void draw();
    void updateProjectionTransform(float width, float height);
    void move(const QVector3D& m);
    void setTranslation(const QVector3D& t);
    void setRotation(const QVector3D& r);
    void setScale(float s);
    void setShadingMode(Drawable::ShadingMode shading);
};

#endif // SCENE_H
