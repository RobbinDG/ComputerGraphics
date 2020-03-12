#include "mesh.h"
#include "model.h"

#include <QImage>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>

Mesh::Mesh(const std::string& filepath, const std::string& texturepath)
    : filepath(filepath), texturepath(texturepath)
{
    load();
    transformMat.setToIdentity();
    transformNormalMat = transformMat.normalMatrix();
}

void Mesh::transform(QMatrix4x4 transform) {
    transformMat = transformMat * transform;
    transformNormalMat = transformMat.normalMatrix();
}

void Mesh::load() {
    Model model(filepath.c_str());
    QVector<float> meshData = model.getVNTInterleaved();

    meshSize = model.getVertices().size();

    // Generate VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(GL_FLOAT), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // Set vertex normals to location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    // Set vertex texture coordinates to location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
