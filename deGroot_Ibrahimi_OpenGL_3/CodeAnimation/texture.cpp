#include "texture.h"

#include <QOpenGLFunctions>

Texture::Texture(QOpenGLFunctions* f, QOpenGLExtraFunctions* ef,  const std::string& filepath) : f(f), ef(ef), path(filepath) {
}

Texture::~Texture() {
    f->glDeleteTextures(1, &name);
}

void Texture::bind() {
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, name);
}

void Texture::load() {
    f->glGenTextures(1, &name);
    // Set texture parameters.
    f->glBindTexture(GL_TEXTURE_2D, name);

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Push image data to texture.
    QImage image(path.c_str());
    QVector<quint8> imageData = imageToBytes(image);

    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
}
