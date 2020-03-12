#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <QOpenGLFunctions_3_3_Core>
#include <QImage>

class Texture
{
private:
    GLuint name;
    std::string path;

public:
    Texture(const std::string& filepath);

private:
    void load();

    // Useful utility method to convert image to bytes.
    QVector<quint8> imageToBytes(QImage image);
};

#endif // TEXTURE_H
