#ifndef MESH_H
#define MESH_H

#include "texture.h"

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Mesh
{
public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

private:
    QOpenGLFunctions* f;
    QOpenGLExtraFunctions* ef;
    GLuint VAO, VBO;
    GLuint meshSize;
    QVector3D translation, globTranslation, rotation, globRotation;
    float scale = 1.0F, globScale = 1.0F;
    QMatrix4x4 transformMat;
    QMatrix3x3 transformNormalMat;
    Texture texture;
    std::string filepath;

    // shading
    QVector4D material = {0.5F, 0.5F, 0.5F, 5.0F};
    QOpenGLShaderProgram phongShaderProgram;

    // Uniforms for the Phong shader program.
    GLint uniformModelViewTransformPhong;
    GLint uniformProjectionTransformPhong;
    GLint uniformNormalTransformPhong;

    GLint uniformMaterialPhong;
    GLint uniformLightPositionPhong;
    GLint uniformLightColorPhong;

    GLint uniformTextureSamplerPhong;

public:
    Mesh(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef, const std::string& filepath, const Texture& texture);
    ~Mesh();
    void setTranslation(const QVector3D& t);
    void setTranslationGlobal(const QVector3D& t);
    void setRotation(const QVector3D& r);
    void setRotationGlobal(const QVector3D& r);
    void setScale(float s);
    void setScaleGlobal(float s);
    void transform(const QVector3D& t, const QVector3D& r, float s);
    void updateTransform();
    void setShadingMode(ShadingMode shading);
    void draw(QVector3D& lightPosition, const QVector3D& lightColor, const QMatrix4x4& projectionTransform);

private:
    void load();
    void createShaderProgram();
    void updatePhongUniforms(QVector3D& lightPosition, const QVector3D& lightColor,
                             const QMatrix4x4& projectionTransform);

private:
    // The current shader to use.
    ShadingMode currentShader = PHONG;
};

#endif // MESH_H
