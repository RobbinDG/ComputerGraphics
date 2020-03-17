#include "animatedmesh.h"

AnimatedMesh::AnimatedMesh(Drawable* mesh, Animation* animation) : mesh(mesh), animation(animation)
{
}

AnimatedMesh::~AnimatedMesh() {
    delete mesh;
    delete animation;
}

void AnimatedMesh::setTranslation(const QVector3D& t) {
    mesh->setTranslation(t);
}

void AnimatedMesh::translate(const QVector3D& t) {
    mesh->translate(t);
}

void AnimatedMesh::setRotation(const QVector3D& r) {
    mesh->setRotation(r);
}

void AnimatedMesh::setScale(float s) {
    mesh->setScale(s);
}

void AnimatedMesh::setTransform(const QVector3D& t, const QVector3D& r, float s) {
    mesh->setTransform(t, r, s);
}

void AnimatedMesh::resetTransform() {
    mesh->resetTransform();
}

void AnimatedMesh::transform(const QVector3D& t, const QVector3D& r, float s) {
    mesh->transform(t, r, s);
}

void AnimatedMesh::transform(const QMatrix4x4& m) {
    mesh->transform(m);
}

void AnimatedMesh::setShadingMode(ShadingMode shading) {
    mesh->setShadingMode(shading);
}

void AnimatedMesh::draw(QVector3D& lightPosition, const QVector3D& lightColor, const QMatrix4x4& projectionTransform) {
    animation->tick();
    animation->animate(reinterpret_cast<Drawable*>(mesh));
    mesh->draw(lightPosition, lightColor, projectionTransform);
}
