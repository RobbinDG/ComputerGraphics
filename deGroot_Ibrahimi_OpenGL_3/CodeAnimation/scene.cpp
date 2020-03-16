#include "scene.h"

Scene::Scene() {}

void Scene::init(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef) {
    glf = f;
    glef = ef;
}

size_t Scene::addMesh(const std::string& meshpath, const std::string& texturepath) {
    Texture texture(glf, glef, texturepath);
    auto mesh = std::make_shared<Mesh>(glf, glef, meshpath, texture);
    _meshes.push_back(std::move(mesh));
    return _meshes.size() - 1;
}

std::shared_ptr<Mesh> Scene::getMesh(size_t idx) {
    return _meshes[idx];
}

void Scene::draw() {
    for (auto& mesh : _meshes) {
        mesh->resetTransform();
        mesh->transform(translation, rotation, scale); // Global transformations
        mesh->draw(lightPosition, lightColor, projectionTransform);
    }
}

void Scene::updateProjectionTransform(float width, float height) {
    float aspectRatio = width / height;
    projectionTransform.setToIdentity();
    projectionTransform.perspective(60.0F, aspectRatio, 0.2F, 20.0F);
}

void Scene::move(const QVector3D& m) {
    translation -= m;
}

void Scene::setTranslation(const QVector3D& t) {
    translation = t;
}

void Scene::setRotation(const QVector3D& r) {
    rotation = r;
}

void Scene::setScale(float s) {
    scale = s;
}

void Scene::setShadingMode(Mesh::ShadingMode shading) {
    for (auto& mesh : _meshes) mesh->setShadingMode(shading);
}
