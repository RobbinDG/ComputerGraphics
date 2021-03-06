#include "mainview.h"
#include "model.h"

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

    glDeleteTextures(1, &textureName);

    destroyModelBuffers();
}

void MainView::initializeWaveArrays() {
    amplitude[0] = 0.0005;
    amplitude[1] = 0.001;
    amplitude[2] = 0.002;
    amplitude[3] = 0.004;
    amplitude[4] = 0.008;
    amplitude[5] = 0.016;
    amplitude[6] = 0.032;
    amplitude[7] = 0.064;
    frequency[0] = 0.05;
    frequency[1] = 0.1;
    frequency[2] = 0.2;
    frequency[3] = 0.4;
    frequency[4] = 0.8;
    frequency[5] = 1.6;
    frequency[6] = 3.2;
    frequency[7] = 6.4;
    phase[0] = 0;
    phase[1] = 0;
    phase[2] = 0;
    phase[3] = 0;
    phase[4] = 0;
    phase[5] = 0;
    phase[6] = 0;
    phase[7] = 0;
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
//    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.2F, 0.5F, 0.7F, 0.0F);

    setupShaders();
    loadMesh();
//    loadTextures();

    // Initialize transformations.
    updateProjectionTransform();
    updateModelTransforms();

    initializeWaveArrays();

    timer.start(1000.0 / 60.0);
}

void MainView::setupShaders() {
    createShaderProgram(0, ":/shaders/vertshader_water.glsl", ":/shaders/fragshader_water.glsl");
    createShaderProgram(1, ":/shaders/vertshader_drop.glsl", ":/shaders/fragshader_drop.glsl");
}

void MainView::createShaderProgram(int idx, const std::string& vert, const std::string& frag) {
    // Create Water shader program.
    shaderProgram[idx].addShaderFromSourceFile(QOpenGLShader::Vertex, vert.c_str());
    shaderProgram[idx].addShaderFromSourceFile(QOpenGLShader::Fragment, frag.c_str());
    shaderProgram[idx].link();

    // Get the uniforms for the shader program.
    uniformModelViewTransform[idx]  = shaderProgram[idx].uniformLocation("modelViewTransform");
    uniformProjectionTransform[idx] = shaderProgram[idx].uniformLocation("projectionTransform");
    uniformNormalTransform[idx]     = shaderProgram[idx].uniformLocation("normalTransform");
    uniformMaterial[idx]            = shaderProgram[idx].uniformLocation("material");
    uniformLightPosition[idx]       = shaderProgram[idx].uniformLocation("lightPosition");
    uniformLightColor[idx]          = shaderProgram[idx].uniformLocation("lightColor");
    uniformTextureSampler[idx]      = shaderProgram[idx].uniformLocation("textureSampler");
    uniformAmplitude[idx]           = shaderProgram[idx].uniformLocation("amplitude");
    uniformFrequency[idx]           = shaderProgram[idx].uniformLocation("frequency");
    uniformPhase[idx]               = shaderProgram[idx].uniformLocation("phase");
    uniformTime[idx]                = shaderProgram[idx].uniformLocation("time");
}

void MainView::loadMesh() {
    Model model(":/models/grid.obj");
    QVector<float> meshData = model.getVNTInterleaved();

    meshSize = model.getVertices().size();

    // Generate VAO
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);

    // Generate VBO
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    // Write the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(GL_FLOAT), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // Set vertex normals to location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    // Set vertex texture coordinates to location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), reinterpret_cast<void*>(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MainView::loadTextures() {
    glGenTextures(1, &textureName);
    loadTexture(":/textures/cat_diff.png", textureName);
}

void MainView::loadTexture(QString file, GLuint textureName) {
    // Set texture parameters.
    glBindTexture(GL_TEXTURE_2D, textureName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Push image data to texture.
    QImage image(file);
    QVector<quint8> imageData = imageToBytes(image);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
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

    // Update the time
    time += 1.0/60.0;

    // Choose the selected shader.
    shaderProgram[currentShader].bind();
    updateUniforms();

    // Set the texture and draw the mesh. Disabled because unnecessary for this project
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, textureName);

    glBindVertexArray(meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, meshSize);

    shaderProgram[currentShader].release();
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

void MainView::updateUniforms() {
    glUniformMatrix4fv(uniformProjectionTransform[currentShader], 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformModelViewTransform[currentShader], 1, GL_FALSE, meshTransform.data());
    glUniformMatrix3fv(uniformNormalTransform[currentShader], 1, GL_FALSE, meshNormalTransform.data());

    glUniform4fv(uniformMaterial[currentShader], 1, &material[0]);
    glUniform3fv(uniformLightPosition[currentShader], 1, &lightPosition[0]);
    glUniform3f(uniformLightColor[currentShader], lightColor.x(), lightColor.y(), lightColor.z());

    glUniform1i(uniformTextureSampler[currentShader], 0);

    glUniform1fv(uniformAmplitude[currentShader], NUMBER_OF_WAVES, amplitude);
    glUniform1fv(uniformFrequency[currentShader], NUMBER_OF_WAVES, frequency);
    glUniform1fv(uniformPhase[currentShader], NUMBER_OF_WAVES, phase);

    glUniform1f(uniformTime[currentShader], time);
}

void MainView::updateProjectionTransform() {
    float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    projectionTransform.setToIdentity();
    projectionTransform.perspective(60.0F, aspectRatio, 0.2F, 20.0F);
}

void MainView::updateModelTransforms() {
    meshTransform.setToIdentity();
    meshTransform.translate(0.0F, -1.0F, -4.0F);

    meshTransform.rotate(rotation.x(), {1.0F, 0.0F, 0.0F});
    meshTransform.rotate(rotation.y(), {0.0F, 1.0F, 0.0F});
    meshTransform.rotate(rotation.z(), {0.0F, 0.0F, 1.0F});

    meshTransform.scale(scale);

    meshNormalTransform = meshTransform.normalMatrix();

    update();
}

// --- OpenGL cleanup helpers

void MainView::destroyModelBuffers() {
    glDeleteBuffers(1, &meshVBO);
    glDeleteVertexArrays(1, &meshVAO);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    rotation = { static_cast<float>(rotateX),
                 static_cast<float>(rotateY),
                 static_cast<float>(rotateZ) };

    updateModelTransforms();
}

void MainView::setScale(int newScale) {
    scale = static_cast<float>(newScale) / 100.0F;
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
