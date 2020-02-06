#include "mainview.h"
#include "Vertex.h"

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";
}

MainView::~MainView() {
    qDebug() << "MainView destructor";

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


}

void MainView::resizeGL(int newWidth, int newHeight) {
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

void MainView::paintGL() {

}

void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
    qDebug() << " → Log:" << Message;
}
