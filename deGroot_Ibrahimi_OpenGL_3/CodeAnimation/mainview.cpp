#include "mainview.h"
#include "model.h"
#include "scene.h"

#include <iostream>
#include <QOpenGLFunctions>

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

    connect(&debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
             this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        qDebug() << ":: Logging initialized";
        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }

    QString glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.2F, 0.5F, 0.7F, 0.0F);

    QOpenGLFunctions *glf = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *glef = QOpenGLContext::currentContext()->extraFunctions();
    scene.init(glf, glef);

    size_t cat = scene.addMesh(":/models/cat.obj", ":/textures/cat_diff.png");
    scene.getMesh(cat)->transform({2.0F, 0.0F, -2.0F}, {0.0f, 0.0f, 0.0f}, 1.0F);

    size_t cat2 = scene.addMesh(":/models/cat.obj", ":/textures/cat_diff.png");
    scene.getMesh(cat2)->transform({-2.0F, 0.0F, -2.0F}, {0.0f, 0.0f, 0.0f}, 1.0F);

    scene.setTranslation({0.0, 0.0, -5.0});

    // Initialize transformations.
    scene.updateProjectionTransform(width(), height());
//    angle = 90.0 / 60.0; // degrees / frame
//    rotationAxis = {0.0, 1.0, 0.0};

    // Start timer
    timer.start(1000.0 / 60.0);
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

    // Choose the selected shader.
    /*
    switch (currentShader) {
    case NORMAL:
        qDebug() << "Normal shader program not implemented";
        break;
    case GOURAUD:
        qDebug() << "Gouraud shader program not implemented";
        break;
    case PHONG:
        phongShaderProgram.bind();
        updatePhongUniforms();
        break;
    }
    */


    // Apply transformations
//    scene.getMesh(0)->transform()
//    meshTransform.translate(translation);
//    meshTransform.rotate(angle, rotationAxis);

    scene.draw();
    update();
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
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
    scene.updateProjectionTransform(static_cast<float>(width()), static_cast<float>(height()));
}

// --- Public interface


void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    scene.setRotation({ static_cast<float>(rotateX),
                 static_cast<float>(rotateY),
                 static_cast<float>(rotateZ) });
}

void MainView::setScale(int newScale) {
    scene.setScale(static_cast<float>(newScale) / 100.0F);
}

void MainView::setShadingMode(Mesh::ShadingMode shading) {
    scene.setShadingMode(shading);
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
