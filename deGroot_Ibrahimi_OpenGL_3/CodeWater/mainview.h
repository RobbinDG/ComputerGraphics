#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QImage>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVector>
#include <QVector3D>

#include <memory>

#define NUMBER_OF_WAVES 8
#define NUMBER_OF_SHADERS 2

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger debugLogger;
    QTimer timer; // Timer used for animation.

    QOpenGLShaderProgram shaderProgram;

    // Uniforms for the shader program.
    GLint uniformModelViewTransform;
    GLint uniformProjectionTransform;
    GLint uniformNormalTransform;

    GLint uniformMaterial;
    GLint uniformLightPosition;
    GLint uniformLightColor;

    GLint uniformTextureSampler;

    GLint uniformAmplitude;
    GLint uniformFrequency;
    GLint uniformPhase;
    GLint uniformTime;

    // Buffers
    GLuint meshVAO;
    GLuint meshVBO;
    GLuint meshSize;

    // Texture
    GLuint textureName;

    GLfloat time;

    // Transforms
    float scale = 1.0F;
    QVector3D rotation;
    QMatrix4x4 projectionTransform;
    QMatrix3x3 meshNormalTransform;
    QMatrix4x4 meshTransform;

    // Phong model constants.
    QVector4D material = {0.2F, 0.8F, 0.1F, 1.0F};
    QVector3D lightPosition = {0.0F, 100.0F, 0.0F};
    QVector3D lightColor = {1.0F, 1.0F, 1.0F};

public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events.
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);

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
    void onMessageLogged( QOpenGLDebugMessage Message );

private:
    void createShaderProgram();
    void loadMesh();

    // Loads texture data into the buffer with the name textureName.
    void loadTextures();
    void loadTexture(QString file, GLuint texturePtr);

    void destroyModelBuffers();

    void updateProjectionTransform();
    void updateModelTransforms();

    void updateUniforms();

    void initializeWaveArrays();

    // Useful utility method to convert image to bytes.
    QVector<quint8> imageToBytes(QImage image);

    // The current shader to use.
    ShadingMode currentShader = PHONG;

    // Wave arrays
    GLfloat amplitude[NUMBER_OF_WAVES];
    GLfloat frequency[NUMBER_OF_WAVES];
    GLfloat phase[NUMBER_OF_WAVES];
};

#endif // MAINVIEW_H
