#include "mesh.h"
#include "model.h"

#include <iostream>

#include <QImage>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLExtraFunctions>

Mesh::Mesh(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef, const std::string& filepath, const Texture& texture)
    : f(f), ef(ef), texture(texture), filepath(filepath)
{
    load();
    this->texture.load();
    createShaderProgram();
    transformMat.setToIdentity();
    transformNormalMat = transformMat.normalMatrix();
}

Mesh::~Mesh() {
    f->glDeleteBuffers(1, &VBO);
    ef->glDeleteVertexArrays(1, &VAO);
}

void Mesh::setTranslation(const QVector3D& t) {
    translation = t;
}

void Mesh::setTranslationGlobal(const QVector3D& t) {
    globTranslation = t;
}

void Mesh::setRotation(const QVector3D& r) {
    rotation = r;
}

void Mesh::setRotationGlobal(const QVector3D& r) {
    globRotation = r;
}

void Mesh::setScale(float s) {
    scale = s;
}

void Mesh::setScaleGlobal(float s) {
    globScale = s;
}

void Mesh::transform(const QVector3D& t, const QVector3D& r, float s) {
    setTranslation(t);
    setRotation(r);
    setScale(s);
}

void Mesh::updateTransform() {
    transformMat.setToIdentity();

    transformMat.translate(globTranslation);

    transformMat.rotate(globRotation.x(), {1.0F, 0.0F, 0.0F});
    transformMat.rotate(globRotation.y(), {0.0F, 1.0F, 0.0F});
    transformMat.rotate(globRotation.z(), {0.0F, 0.0F, 1.0F});

    transformMat.scale(globScale);

    transformMat.translate(translation);

    transformMat.rotate(rotation.x(), {1.0F, 0.0F, 0.0F});
    transformMat.rotate(rotation.y(), {0.0F, 1.0F, 0.0F});
    transformMat.rotate(rotation.z(), {0.0F, 0.0F, 1.0F});

    transformMat.scale(scale);

    transformNormalMat = transformMat.normalMatrix();
}

void Mesh::draw(QVector3D& lightPosition, const QVector3D& lightColor, const QMatrix4x4& projectionTransform) {
    updateTransform();

    phongShaderProgram.bind();
    updatePhongUniforms(lightPosition, lightColor, projectionTransform);

    texture.bind();

    ef->glBindVertexArray(VAO);

    f->glDrawArrays(GL_TRIANGLES, 0, meshSize);

    phongShaderProgram.release();
}

void Mesh::load() {
    Model model(filepath.c_str());
    QVector<float> meshData = model.getVNTInterleaved();

    meshSize = model.getVertices().size();

    // Generate VAO
    ef->glGenVertexArrays(1, &VAO);
    ef->glBindVertexArray(VAO);

    // Generate VBO
    f->glGenBuffers(1, &VBO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data to the buffer
    f->glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(GL_FLOAT), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(0));
    f->glEnableVertexAttribArray(0);

    // Set vertex normals to location 1
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    f->glEnableVertexAttribArray(1);

    // Set vertex texture coordinates to location 2
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(6 * sizeof(GL_FLOAT)));
    f->glEnableVertexAttribArray(2);

    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    ef->glBindVertexArray(0);
}

void Mesh::createShaderProgram() {
    // Create Phong shader program.
    phongShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_phong.glsl");
    phongShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_phong.glsl");
    phongShaderProgram.link();

    // Get the uniforms for the Phong shader program.
    uniformModelViewTransformPhong  = phongShaderProgram.uniformLocation("modelViewTransform");
    uniformProjectionTransformPhong = phongShaderProgram.uniformLocation("projectionTransform");
    uniformNormalTransformPhong     = phongShaderProgram.uniformLocation("normalTransform");
    uniformMaterialPhong            = phongShaderProgram.uniformLocation("material");
    uniformLightPositionPhong       = phongShaderProgram.uniformLocation("lightPosition");
    uniformLightColorPhong          = phongShaderProgram.uniformLocation("lightColor");
    uniformTextureSamplerPhong      = phongShaderProgram.uniformLocation("textureSampler");
}

void Mesh::updatePhongUniforms(QVector3D& lightPosition, const QVector3D& lightColor,
                               const QMatrix4x4& projectionTransform) {
    f->glUniformMatrix4fv(uniformProjectionTransformPhong, 1, GL_FALSE, projectionTransform.data());
    f->glUniformMatrix4fv(uniformModelViewTransformPhong, 1, GL_FALSE, transformMat.data());
    f->glUniformMatrix3fv(uniformNormalTransformPhong, 1, GL_FALSE, transformNormalMat.data());

    f->glUniform4fv(uniformMaterialPhong, 1, &material[0]);
    f->glUniform3fv(uniformLightPositionPhong, 1, &lightPosition[0]);
    f->glUniform3f(uniformLightColorPhong, lightColor.x(), lightColor.y(), lightColor.z());

    f->glUniform1i(uniformTextureSamplerPhong, 0);
}

void Mesh::setShadingMode(ShadingMode shading) {
    qDebug() << "Changed shading to" << shading;
    currentShader = shading;
}
