#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <QOpenGLFunctions_3_3_Core>
#include <QImage>

/**
 * @brief The Texture class
 * A self-allocating and destructing texture class.
 */
class Texture
{
private:
    QOpenGLFunctions* f;
    QOpenGLExtraFunctions* ef;
    GLuint name;
    std::string path;

public:
    Texture(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef,  const std::string& filepath);
    ~Texture();
    void bind();
    void load();

private:

    // Useful utility method to convert image to bytes.
    QVector<quint8> imageToBytes(QImage image);
};

#endif // TEXTURE_H
