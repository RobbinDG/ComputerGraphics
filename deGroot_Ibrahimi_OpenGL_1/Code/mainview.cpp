#include "Vertex.h"
#include "mainview.h"
#include "model.h"
#include <math.h>
#include <QDateTime>
#include <iostream>
#include <stdlib.h>
#include <time.h>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    qDebug() << "MainView destructor";
    glDeleteBuffers(1, &VBOcube);
    glDeleteVertexArrays(1, &VAOcube);
    makeCurrent();
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    connect(&debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage)),
            this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        qDebug() << ":: Logging initialized";
        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color to be used by glClear. This is, effectively, the background color.
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();

    Vertex cube[] = {
        {1,1,1,1,0,0},
        {1,1,-1,0,1,0},
        {1,-1,1,0,0,1},
        {1,-1,-1,1,0,0},
        {-1,1,1,0,1,0},
        {-1,1,-1,0,0,1},
        {-1,-1,1,1,0,0},
        {-1,-1,-1,0,1,0}
    };

    Vertex pyramid[] = {
        {0,1,0,1,0,0},
        {1,-1,1,0,1,0},
        {1,-1,-1,0,0,1},
        {-1,-1,1,1,0,0},
        {-1,-1,-1,0,1,0},
    };

    Vertex triangulatedCube[] {
        cube[0], cube[3], cube[1],
        cube[0], cube[2], cube[3],
        cube[0], cube[6], cube[2],
        cube[0], cube[4], cube[6],
        cube[4], cube[5], cube[6],
        cube[5], cube[7], cube[6],
        cube[1], cube[3], cube[7],
        cube[1], cube[7], cube[5],
        cube[0], cube[1], cube[5],
        cube[1], cube[5], cube[4],
        cube[2], cube[6], cube[3],
        cube[3], cube[6], cube[7],
    };

    Vertex triangulatedPyramid[] = {
        pyramid[0], pyramid[1], pyramid[2],
        pyramid[0], pyramid[3], pyramid[1],
        pyramid[0], pyramid[4], pyramid[3],
        pyramid[0], pyramid[2], pyramid[4],
        pyramid[1], pyramid[3], pyramid[4],
        pyramid[1], pyramid[4], pyramid[2],
    };

    cubeTranslate = QMatrix4x4(
                1,0,0,2,
                0,1,0,0,
                0,0,1,-6,
                0,0,0,1
    );

    pyrTranslate = QMatrix4x4(
                1,0,0,-2,
                0,1,0,0,
                0,0,1,-6,
                0,0,0,1
    );

    projection.perspective(60, 16.0f/9.0f, 0.01f, 10);

    setupVBOVAO(VBOcube, VAOcube, triangulatedCube, 12);
    setupVBOVAO(VBOpyr, VAOpyr, triangulatedPyramid, 6);

    scaleFactor = 1.0;
    rotX = rotY = rotZ = 0.0;

//    srand(time(NULL));

//    Model sphere = Model(":/models/sphere.obj");
//    QVector<QVector3D> coordinates = sphere.getVertices();
//    int size = coordinates.count();
//    Vertex sphereVertices[size];

//    qDebug() << "Size: " << size;

//    int pos = 0;
//    for (QVector3D vector : coordinates) {
//        sphereVertices[pos].x = vector[0];
//        sphereVertices[pos].y = vector[1];
//        sphereVertices[pos].z = vector[2];
//        sphereVertices[pos].r = rand() % 255 + 1;
//        sphereVertices[pos].g = rand() % 255 + 1;
//        sphereVertices[pos].b = rand() % 255 + 1;
//    }

//    setupVBOVAO(VBOsphere, VAOsphere, sphereVertices, size);
}

void MainView::setupVBOVAO(GLuint& VBO, GLuint& VAO, Vertex shape[], int triangles) {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangles * 3 * sizeof(Vertex), shape, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
}

void MainView::createShaderProgram() {
    // Create shader program
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader.glsl");
    shaderProgram.link();

    modelTransform = shaderProgram.uniformLocation("modelTransform");
    projectTransform = shaderProgram.uniformLocation("projectTransform");
    std::cout << "modelTransform: " << modelTransform << ", projectTransform: " << projectTransform << std::endl;
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    // Update Project transform
    glUniformMatrix4fv(projectTransform, 1, GL_FALSE, projection.data());

    // First store the translation in the final matrices
    QMatrix4x4 updatedCube = cubeTranslate;
    QMatrix4x4 updatedPyr = pyrTranslate;

    // Scale the matrices according to the scaling factor
    updatedCube.scale(scaleFactor);
    updatedPyr.scale(scaleFactor);

    // Rotate the matrices according the rotation parameters
    updatedCube.rotate(angle, rotX, rotY, rotZ);
    updatedPyr.rotate(angle, rotX, rotY, rotZ);

    // Update Model transform and draw the cube using the final matrices
    glUniformMatrix4fv(modelTransform, 1, GL_FALSE, updatedCube.data());
    glBindVertexArray(VAOcube);
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    // Update Model transform and draw the pyramid using the final matrices
    glUniformMatrix4fv(modelTransform, 1, GL_FALSE, updatedPyr.data());
    glBindVertexArray(VAOpyr);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 3);

//    glBindVertexArray(VAOsphere);
//    glDrawArrays(GL_TRIANGLES, 0, 2280 * 3);

    shaderProgram.release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {
    // First reset the QMatrix4x4 projection to identity, and get the new perspective projection matrix
    projection.setToIdentity();
    projection.perspective(60, (float)newWidth/newHeight, 0.01f, 10.0);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    if (rotX != rotateX) {
        angle = rotateX;
    }
    else if (rotY != rotateY) {
        angle = rotateY;
    }
    else {
        angle = rotateZ;
    }
    rotX = rotateX;
    rotY = rotateY;
    rotZ = rotateZ;
    update();
//    Q_UNIMPLEMENTED();
}

void MainView::setScale(int scale) {
    // Ensures that the scaleFactor is between 0.5 and 1.5
    scaleFactor = 1 + (scale - 100.0) * 5 / 1000.0;

    update();
}

void MainView::setShadingMode(ShadingMode shading) {
    qDebug() << "Changed shading to" << shading;
    Q_UNIMPLEMENTED();
}

// --- Private helpers

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}

