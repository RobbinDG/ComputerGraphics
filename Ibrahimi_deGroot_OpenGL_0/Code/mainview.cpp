#include "mainview.h"
#include "Vertex.h"

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";
}

MainView::~MainView() {
    qDebug() << "MainView destructor";
    glDeleteBuffers(1, &VBOname);
    glDeleteVertexArrays(1, &VAOname);
    makeCurrent();
}

void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    connect(&debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
            this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        qDebug() << ":: Logging initialized";
        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    Vertex r = {-1, -1, 1, 0, 0};
    Vertex g = {1, -1, 0, 1, 0};
    Vertex b = {0, 1, 0, 0, 1};

    Vertex vertices[3] = {r, g, b};

    glGenBuffers(1, &VBOname);
    glGenVertexArrays(1, &VAOname);

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    shader.link();
    shader.bind();

    glBindVertexArray(VAOname);
    glBindBuffer(GL_ARRAY_BUFFER, VBOname);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(2 * sizeof(float)));

}

void MainView::resizeGL(int newWidth, int newHeight) {
    Q_UNUSED(newWidth);
    Q_UNUSED(newHeight);
}

void MainView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glBindVertexArray(VAOname);
    shader.bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
    qDebug() << " → Log:" << Message;
}
