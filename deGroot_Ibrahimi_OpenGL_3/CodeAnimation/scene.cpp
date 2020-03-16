#include "scene.h"

Scene::Scene() {}

Scene::~Scene() {
    for (auto& obj : _objects) {
        delete obj;
    }
}

void Scene::init(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef) {
    glf = f;
    glef = ef;
}

size_t Scene::addObject(Drawable *obj) {
    _objects.push_back(obj);
    return _objects.size() - 1;
}

size_t Scene::addMesh(const std::string& meshpath, const std::string& texturepath) {
    Texture texture(glf, glef, texturepath);
    auto mesh = new Mesh(glf, glef, meshpath, texture);
    _objects.push_back(reinterpret_cast<Drawable*>(mesh));
    return _objects.size() - 1;
}

Drawable* Scene::getObject(size_t idx) {
    return _objects[idx];
}

void Scene::draw() {
    for (auto& object : _objects) {
        object->resetTransform();
        object->transform(translation, rotation, scale); // Global transformations
        object->draw(lightPosition, lightColor, projectionTransform);
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

void Scene::setShadingMode(Drawable::ShadingMode shading) {
    for (auto& obj : _objects) obj->setShadingMode(shading);
}
