#include "mainview.h"
#include "model.h"
#include "vertex.h"
#include "QVector3D"

#include <QDateTime>


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

    destroyModelBuffers();
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

    currentShader = 0;

    createShaderProgram();
    loadMesh();
    loadTexture(":/textures/cat_diff.png", texturePtr);

    // Initialize transformations
    updateProjectionTransform();
    updateModelTransforms();
}

void MainView::createShaderProgram() {
    // Create shader program
    shaderPrograms[0].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_phong.glsl");
    shaderPrograms[0].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_phong.glsl");
    shaderPrograms[1].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_normal.glsl");
    shaderPrograms[1].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_normal.glsl");
    shaderPrograms[2].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_gouraud.glsl");
    shaderPrograms[2].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_gouraud.glsl");

    shaderPrograms[0].link();
    shaderPrograms[1].link();
    shaderPrograms[2].link();


    // Get the Phong uniforms
    uniformPhongModelViewTransform = shaderPrograms[0].uniformLocation("modelViewTransform");
    uniformPhongProjectionTransform = shaderPrograms[0].uniformLocation("projectionTransform");
    uniformPhongNormalTransform = shaderPrograms[0].uniformLocation("transNorms");
    uniformPhongMaterial = shaderPrograms[0].uniformLocation("material");
    uniformPhongSampler = shaderPrograms[0].uniformLocation("samplerUniform");

    // Get the Normal uniforms
    uniformNormalModelViewTransform = shaderPrograms[1].uniformLocation("modelViewTransform");
    uniformNormalProjectionTransform = shaderPrograms[1].uniformLocation("projectionTransform");
    uniformNormalNormalTransform = shaderPrograms[1].uniformLocation("transNorms");
    uniformNormalMaterial = shaderPrograms[1].uniformLocation("material");
    uniformNormalSampler = shaderPrograms[1].uniformLocation("samplerUniform");

    // Get the Gouraud uniforms
    uniformGouraudModelViewTransform = shaderPrograms[2].uniformLocation("modelViewTransform");
    uniformGouraudProjectionTransform = shaderPrograms[2].uniformLocation("projectionTransform");
    uniformGouraudNormalTransform = shaderPrograms[2].uniformLocation("transNorms");
    uniformGouraudMaterial = shaderPrograms[2].uniformLocation("material");
    uniformGouraudSampler = shaderPrograms[2].uniformLocation("samplerUniform");
}

void MainView::loadMesh() {
    Model model(":/models/cat.obj");
    model.unitize();

    QVector<QVector3D> normals = model.getNormals();
    QVector<QVector3D> vertexCoords = model.getVertices();
    QVector<QVector2D> textureCoords = model.getTextureCoords();

    QVector<float> meshData;
    meshData.reserve((3 + 3 + 2) * vertexCoords.size());

    for (int i = 0; i < vertexCoords.size(); i++) {
        auto coord = vertexCoords[i];
        auto normal = normals[i];
        auto texture = textureCoords[i];

        meshData.append(coord.x());
        meshData.append(coord.y());
        meshData.append(coord.z());

        meshData.append(normal.x());
        meshData.append(normal.y());
        meshData.append(normal.z());

        meshData.append(texture.x());
        meshData.append(texture.y());
    }

    meshSize = vertexCoords.size();

    // Generate VAO
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);

    // Generate VBO
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    // Write the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(float), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Set colour coordinates to location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture coordinates to location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MainView::loadTexture(QString file, GLuint texturePtr) {
    glBindTexture(GL_TEXTURE_2D, texturePtr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    QImage image = QImage(file);
    QVector<quint8> imageBytes = imageToBytes(image);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    QMatrix3x3 transNorms = meshTransform.normalMatrix();
    QVector3D material(0.3, 1, 0.2);

    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderPrograms[currentShader].bind();

    // Set the projection matrix for the current shader
    glUniformMatrix4fv(uniformPhongProjectionTransform, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformPhongModelViewTransform, 1, GL_FALSE, meshTransform.data());
    glUniformMatrix3fv(uniformPhongNormalTransform, 1, GL_FALSE, transNorms.data());
    glUniform3fv(uniformPhongMaterial, 1, reinterpret_cast<GLfloat *>(&material[0]));
    glUniform1i(uniformPhongSampler, 0);

    glUniformMatrix4fv(uniformNormalProjectionTransform, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformNormalModelViewTransform, 1, GL_FALSE, meshTransform.data());
    glUniformMatrix3fv(uniformNormalNormalTransform, 1, GL_FALSE, transNorms.data());
    glUniform3fv(uniformNormalMaterial, 1, reinterpret_cast<GLfloat *>(&material[0]));
    glUniform1i(uniformNormalSampler, 0);

    glUniformMatrix4fv(uniformGouraudProjectionTransform, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformGouraudModelViewTransform, 1, GL_FALSE, meshTransform.data());
    glUniformMatrix3fv(uniformGouraudNormalTransform, 1, GL_FALSE, transNorms.data());
    glUniform3fv(uniformGouraudMaterial, 1, reinterpret_cast<GLfloat *>(&material[0]));
    glUniform1i(uniformGouraudSampler, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturePtr);

    glBindVertexArray(meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, meshSize);

    shaderPrograms[currentShader].release();
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
    updateProjectionTransform();
}

void MainView::updateProjectionTransform() {
    float aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
    projectionTransform.setToIdentity();
    projectionTransform.perspective(60, aspect_ratio, 0.2, 20);
}

void MainView::updateModelTransforms() {
    meshTransform.setToIdentity();
    meshTransform.translate(0, 0, -10);
    meshTransform.scale(scale);
    meshTransform.rotate(QQuaternion::fromEulerAngles(rotation));

    update();
}

// --- OpenGL cleanup helpers

void MainView::destroyModelBuffers() {
    glDeleteBuffers(1, &meshVBO);
    glDeleteVertexArrays(1, &meshVAO);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    rotation = { static_cast<float>(rotateX), static_cast<float>(rotateY), static_cast<float>(rotateZ) };
    updateModelTransforms();
}

void MainView::setScale(int newScale) {
    scale = static_cast<float>(newScale) / 100.f;
    updateModelTransforms();
}

void MainView::setShadingMode(ShadingMode shading) {
    qDebug() << "Changed shading to" << shading;
    currentShader = shading;
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

