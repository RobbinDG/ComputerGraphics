#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"

#include <vector>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>


class Scene
{
private:
    std::vector<std::shared_ptr<Mesh>> _meshes;

    QOpenGLFunctions* glf;
    QOpenGLExtraFunctions* glef;

    // Phong model constants.
    QVector3D lightPosition = {0.0F, 100.0F, 0.0F};
    QVector3D lightColor = {1.0F, 1.0F, 1.0F};
    QMatrix4x4 projectionTransform;

    // Transforms
    QVector3D translation, rotation;
    float scale = 1.0F;

public:
    Scene();

    /**
     * @brief addMesh creates and adds mesh to scene.
     * @return the mesh index in the scene.
     */
    void init(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef);
    size_t addMesh(const std::string& meshpath, const std::string& texturepath);
    std::shared_ptr<Mesh> getMesh(size_t idx);
    const std::vector<std::shared_ptr<Mesh>> meshes();
    void draw();
    void updateProjectionTransform(float width, float height);
    void move(const QVector3D& m);
    void setTranslation(const QVector3D& t);
    void setRotation(const QVector3D& r);
    void setScale(float s);
    void setShadingMode(Mesh::ShadingMode shading);
};

#endif // SCENE_H
