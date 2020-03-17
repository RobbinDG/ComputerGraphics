#include "mainview.h"
#include "model.h"
#include "scene.h"
#include "animatedmesh.h"
#include "localrotateanimation.h"
#include "backandforthanimation.h"

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

    // Define scene
    Texture catTexture(glf, glef, ":/textures/cat_norm.png");
    auto catmesh = new Mesh(glf, glef, ":/models/cat.obj", catTexture);
    catmesh->setTransform({2.0F, 0.0F, -2.0F}, {0.0f, 0.0f, 0.0f}, 1.0F);
    auto catanimation = new LocalRotateAnimation(90, 4.0, {0.0, 1.0, 0.0});
    auto cat = new AnimatedMesh(catmesh, reinterpret_cast<Animation*>(catanimation));
    scene.addObject(reinterpret_cast<Drawable*>(cat));

    Texture cat2Texture(glf, glef, ":/textures/cat_diff.png");
    auto cat2mesh = new Mesh(glf, glef, ":/models/cat.obj", cat2Texture);
    cat2mesh->setTransform({-2.0F, 0.0F, -2.0F}, {0.0f, 0.0f, 0.0f}, 1.0F);
    auto cat2animation = new LocalRotateAnimation(360, 1.0, {0.0, 1.0, 0.0});
    auto cat2 = new AnimatedMesh(cat2mesh, reinterpret_cast<Animation*>(cat2animation));
    scene.addObject(reinterpret_cast<Drawable*>(cat2));

    Texture cubeTexture(glf, glef, ":/textures/rug_logo.png");
    auto cubemesh = new Mesh(glf, glef, ":/models/cube.obj", cubeTexture);
    cubemesh->setTransform({0.0F, -1.0F, -3.0F}, {0.0f, 90.0f, 0.0f}, 0.5F);
    auto cubeanimation = new BackAndForthAnimation(100, 0.1, {0.0, 1.0, 0.0});
    auto cube = new AnimatedMesh(cubemesh, reinterpret_cast<Animation*>(cubeanimation));
    scene.addObject(reinterpret_cast<Drawable*>(cube));

    Texture cube2Texture(glf, glef, ":/textures/rug_logo.png");
    auto cube2mesh = new Mesh(glf, glef, ":/models/cube.obj", cube2Texture);
    cube2mesh->setTransform({0.0F, -1.0F, -6.0F}, {90.0f, 90.0f, 0.0f}, 0.5F);
    auto cube2animation = new BackAndForthAnimation(1000, 0.02, {1.0, 1.0, 1.0});
    auto cube2 = new AnimatedMesh(cube2mesh, reinterpret_cast<Animation*>(cube2animation));
    scene.addObject(reinterpret_cast<Drawable*>(cube2));

    // Initialize transformations
    scene.setTranslation({0.0, 0.0, -5.0});
    scene.updateProjectionTransform(width(), height());

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

void MainView::setShadingMode(Drawable::ShadingMode shading) {
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
