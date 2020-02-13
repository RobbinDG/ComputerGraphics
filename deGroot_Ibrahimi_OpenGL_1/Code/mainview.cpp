#include "Vertex.h"
#include "mainview.h"

#include <QDateTime>
#include <iostream>

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

//    glEnable(GL_CULL_FACE);

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
        cube[0], cube[1], cube[3],
        cube[0], cube[3], cube[2],
        cube[0], cube[2], cube[6],
        cube[0], cube[6], cube[4],
        cube[4], cube[6], cube[5],
        cube[5], cube[7], cube[6],
        cube[1], cube[7], cube[3],
        cube[1], cube[5], cube[7],
        cube[0], cube[5], cube[1],
        cube[1], cube[4], cube[5],
        cube[2], cube[3], cube[6],
        cube[3], cube[7], cube[6],
    };

    Vertex triangulatedPyramid[] = {
        pyramid[0], pyramid[2], pyramid[1],
        pyramid[0], pyramid[1], pyramid[3],
        pyramid[0], pyramid[3], pyramid[4],
        pyramid[0], pyramid[4], pyramid[2],
        pyramid[1], pyramid[4], pyramid[3],
        pyramid[1], pyramid[2], pyramid[4],
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

    float n = 5.0, f = 7.0, t = 3.0, b = -3.0, l = -4.0, r = 4.0;
    projection = QMatrix4x4(
                2*n/(r-l),0,(r+l)/(r-l),0,
                0,2*n/(t-b),(t+b)/(t-b),0,
                0,0,(n+f)/(n-f),(2*f*n)/(n-f),
                0,0,-1,0
    );

    glGenBuffers(1, &VBOcube);
    glGenBuffers(1, &VBOpyr);
    glGenVertexArrays(1, &VAOcube);
    glGenVertexArrays(1, &VAOpyr);


    glBindVertexArray(VAOcube);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
    glBufferData(GL_ARRAY_BUFFER, 12 * 3 * sizeof(Vertex), triangulatedCube, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));

    glBindVertexArray(VAOpyr);
    glBindBuffer(GL_ARRAY_BUFFER, VBOpyr);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(Vertex), triangulatedPyramid, GL_STATIC_DRAW);

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
    std::cout << modelTransform << " " << projectTransform << std::endl;
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

    glUniformMatrix4fv(projectTransform, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(modelTransform, 1, GL_FALSE, cubeTranslate.data());

    glBindVertexArray(VAOcube);
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    glUniformMatrix4fv(modelTransform, 1, GL_FALSE, pyrTranslate.data());

    glBindVertexArray(VAOpyr);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 3);

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
    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    Q_UNIMPLEMENTED();
}

void MainView::setScale(int scale) {
    qDebug() << "Scale changed to " << scale;
    Q_UNIMPLEMENTED();
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

