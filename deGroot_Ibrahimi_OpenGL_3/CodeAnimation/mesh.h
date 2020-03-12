#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "texture.h"

#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>

class Mesh : QOpenGLFunctions_3_3_Core
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint meshSize;
    QMatrix4x4 transformMat;
    QMatrix3x3 transformNormalMat;
    Texture texture;
    std::string filepath;

public:
    Mesh(const std::string& filepath, const Texture& texture);
    void transform(QMatrix4x4 transform);

private:
    void load();
};

#endif // BUFFERMANAGER_H
