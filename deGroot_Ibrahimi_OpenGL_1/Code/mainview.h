#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"
#include "Vertex.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
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
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation

    GLuint VBOcube, VBOpyr, VBOsphere, VAOcube, VAOpyr, VAOsphere;
    GLfloat scaleFactor, rotX, rotY, rotZ, angle;
    GLint modelTransform, projectTransform;
    QMatrix4x4 cubeTranslate, pyrTranslate, sphereTranslate;
    QMatrix4x4 projection;

    void deleteVAOs();
    void deleteVBOs();

    void setupCube();
    void setupPyr();
    void setupSphere();
    void translateObjects();

    void setupVBOVAO(GLuint& VBO, GLuint& VAO, Vertex shape[], int triangles);
    QMatrix4x4 createTransformation(QMatrix4x4 translated, float scaleOffset);
    void paintObject(QMatrix4x4 transformation, GLuint VAO, int amountOfTriangles);
    void setProjection(float left, float right, float bottom, float top, float zNear, float zFar);

    QOpenGLShaderProgram shaderProgram;

    void createShaderProgram();
};

#endif // MAINVIEW_H
