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

    scaleFactor = 1.0;
    rotX = rotY = rotZ = 0.0;

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

void MainView::deleteVAOs() {
    glDeleteVertexArrays(1, &VAOcube);
    glDeleteVertexArrays(1, &VAOpyr);
    glDeleteVertexArrays(1, &VAOsphere);
}

void MainView::deleteVBOs() {
    glDeleteBuffers(1, &VBOcube);
    glDeleteBuffers(1, &VBOpyr);
    glDeleteBuffers(1, &VBOsphere);
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
    deleteVBOs();
    deleteVAOs();
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

    // Set all objects
    setupCube();
    setupPyr();
    setupSphere();

    // Translate all objects
    translateObjects();

    // Set the projection perspective
    projection.perspective(60, 16.0f/9.0f, 0.01f, 10);
}

/**
 * @brief MainView::setupCube Sets the cube
 */
void MainView::setupCube() {
    std::vector<Vertex> cube = {
        {1,1,1,1,0,0},
        {1,1,-1,0,1,0},
        {1,-1,1,0,0,1},
        {1,-1,-1,1,0,0},
        {-1,1,1,0,1,0},
        {-1,1,-1,0,0,1},
        {-1,-1,1,1,0,0},
        {-1,-1,-1,0,1,0}
    };

    std::vector<Vertex> triangulatedCube {
        // right
        cube[0], cube[3], cube[1],
        cube[0], cube[2], cube[3],
        // back
        cube[0], cube[6], cube[2],
        cube[0], cube[4], cube[6],
        // left
        cube[4], cube[5], cube[6],
        cube[5], cube[7], cube[6],
        // front
        cube[1], cube[3], cube[7],
        cube[1], cube[7], cube[5],
        // top
        cube[0], cube[1], cube[5],
        cube[0], cube[5], cube[4],
        // bottom
        cube[2], cube[6], cube[3],
        cube[3], cube[6], cube[7],
    };

    setupVBOVAO(VBOcube, VAOcube, triangulatedCube.data(), 3 * 12);
}

/**
 * @brief MainView::setupPyr Sets the pyramid
 */
void MainView::setupPyr() {
    std::vector<Vertex> pyramid = {
        {0,1,0,1,0,0},
        {1,-1,1,0,1,0},
        {1,-1,-1,0,0,1},
        {-1,-1,1,1,0,0},
        {-1,-1,-1,0,1,0},
    };

    std::vector<Vertex> triangulatedPyramid = {
        pyramid[0], pyramid[1], pyramid[2],
        pyramid[0], pyramid[3], pyramid[1],
        pyramid[0], pyramid[4], pyramid[3],
        pyramid[0], pyramid[2], pyramid[4],
        pyramid[1], pyramid[3], pyramid[4],
        pyramid[1], pyramid[4], pyramid[2],
    };

    setupVBOVAO(VBOpyr, VAOpyr, triangulatedPyramid.data(), 3 * 6);
}

/**
 * @brief MainView::setupSphere Sets the sphere
 */
void MainView::setupSphere() {
    srand(time(NULL));
    Model sphere = Model(":/models/sphere.obj");
    QVector<QVector3D> coordinates = sphere.getVertices();
    std::vector<Vertex> sphereVertices;
    for (QVector3D vector : coordinates) {
        sphereVertices.push_back({vector.x(), vector.y(), vector.z(), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)});
    }

    setupVBOVAO(VBOsphere, VAOsphere, sphereVertices.data(), coordinates.count());
}

/**
 * @brief MainView::translateObjects Translates all objects to the appropriate position
 */
void MainView::translateObjects() {
    // Cube
    cubeTranslate.setToIdentity();
    cubeTranslate.translate(2, 0, -6);

    // Pyramid
    pyrTranslate.setToIdentity();
    pyrTranslate.translate(-2, 0, -6);

    // Sphere
    sphereTranslate.setToIdentity();
    sphereTranslate.translate(0, 0, -10);
}

/**
 * @brief MainView::setupVBOVAO Sets the VBO and VAO of an object
 * @param VBO The VBO of the shape
 * @param VAO The VAO of the shape
 * @param shape The shape (expressed as a Vertex array)
 * @param triangles The amount of triangles
 */
void MainView::setupVBOVAO(GLuint& VBO, GLuint& VAO, Vertex shape[], int triangles) {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangles * sizeof(Vertex), shape, GL_STATIC_DRAW);

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

    // Create the final matrices from the translated cubes
    QMatrix4x4 transformCube = createTransformation(cubeTranslate, 1);
    QMatrix4x4 transformdPyr = createTransformation(pyrTranslate, 1);
    QMatrix4x4 transformSphere = createTransformation(sphereTranslate, 0.02);


    // Update Model transform and draw the object using the final transformation matrices
    paintObject(transformCube, VAOcube, 12 * 3);
    paintObject(transformdPyr, VAOpyr, 6 * 3);
    paintObject(transformSphere, VAOsphere, 760 * 3);

    shaderProgram.release();
}

void MainView::paintObject(QMatrix4x4 transformation, GLuint VAO, int amountOfTriangles) {
    // Update Project transform
    glUniformMatrix4fv(projectTransform, 1, GL_FALSE, projection.data());

    // Update modelTransform and draw the object
    glUniformMatrix4fv(modelTransform, 1, GL_FALSE, transformation.data());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, amountOfTriangles);
}

QMatrix4x4 MainView::createTransformation(QMatrix4x4 translated, float scaleOffset) {
    // Scale the matrix according to the scaling factor and offset
    translated.scale(scaleFactor * scaleOffset);

    // Rotate the matrix according the rotation parameters
    translated.rotate(rotX, 1, 0, 0);
    translated.rotate(rotY, 0, 1, 0);
    translated.rotate(rotZ, 0, 0, 1);

    return translated;
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
    rotX = rotateX;
    rotY = rotateY;
    rotZ = rotateZ;
    update();
}

void MainView::setScale(int scale) {
    // Ensures that the scaleFactor is between 0.8 and 1.2
    scaleFactor = 1 + (scale - 100.0) * 2 / 1000.0;

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

