#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <QMatrix4x4>
#include <memory>


class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    enum ShadingMode : GLuint {
        PHONG = 0, NORMAL, GOURAUD
    };

    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);
    QVector<quint8> imageToBytes(QImage image);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private slots:
    void onMessageLogged(QOpenGLDebugMessage Message);

private:
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation

    GLint currentShader;
    QOpenGLShaderProgram shaderPrograms[3];

    // Phong uniforms
    GLint uniformPhongModelViewTransform;
    GLint uniformPhongProjectionTransform;
    GLint uniformPhongNormalTransform;
    GLint uniformPhongMaterial;
    GLint uniformPhongSampler;

    // Normal uniforms
    GLint uniformNormalModelViewTransform;
    GLint uniformNormalProjectionTransform;
    GLint uniformNormalNormalTransform;
    GLint uniformNormalMaterial;
    GLint uniformNormalSampler;

    // Gouraud uniforms
    GLint uniformGouraudModelViewTransform;
    GLint uniformGouraudProjectionTransform;
    GLint uniformGouraudNormalTransform;
    GLint uniformGouraudMaterial;
    GLint uniformGouraudSampler;

    // Mesh values
    GLuint meshVAO;
    GLuint meshVBO;
    GLuint meshSize;
    QMatrix4x4 meshTransform;

    // Texture values
    GLuint texturePtr;

    // Transforms
    float scale = 1.f;
    QVector3D rotation;
    QMatrix4x4 projectionTransform;

    void createShaderProgram();
    void loadMesh();
    void loadTexture(QString file, GLuint texturePtr);

    void destroyModelBuffers();

    void updateProjectionTransform();
    void updateModelTransforms();
};

#endif // MAINVIEW_H
