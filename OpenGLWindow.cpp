// OpenGLWindow.cpp
#include "stdafx.h"
#include "OpenGLWindow.h"
#include "Mesh.h"
#include "Texture2D.h"
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QFileInfo>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <GL/gl.h> 



OpenGLWindow::OpenGLWindow(const QColor& background, QMainWindow* parent)
    : mBackground(background)
{
    setParent(parent);
    setMinimumSize(1300, 784);
}

OpenGLWindow::~OpenGLWindow()
{
    reset();
}

void OpenGLWindow::reset()
{
    // And now release all OpenGL resources.
    makeCurrent();
    delete mProgram;
    mProgram = nullptr;
    delete mVshader;
    mVshader = nullptr;
    delete mFshader;
    mFshader = nullptr;
    mVbo.destroy();
    doneCurrent();

    QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::paintGL()
{
        glClear(GL_COLOR_BUFFER_BIT);
     //   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        mProgram->bind();

        QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
        vbo.create();
        vbo.bind();
        vbo.allocate(mVertices.constData(), mVertices.size() * sizeof(mNormals));

        QMatrix4x4 matrix;
        matrix.rotate(rotationAngle);
        matrix.ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, -15);
        matrix.ortho(-30.0f * zoomFactor, 30.0f * zoomFactor, -30.0f * zoomFactor, 30.0f * zoomFactor, 0.1f * zoomFactor, 100.0f * zoomFactor);

        mProgram->setUniformValue(m_matrixUniform, matrix);


        mProgram->bind();
    

        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, mVertices.data());
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColors.data());
        glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, 0, mNormals.data());
        glEnableVertexAttribArray(m_normalAttr);

      

        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);


        // Draw arrays (if necessary)
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size() / 3);

        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);
    


}


void OpenGLWindow::initializeGL()
{/*
    static const char* vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

    static const char* fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";*/

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    QString vertexShaderSource = readShader("VertexShaders.glsl");
    QString fragmentShaderSource = readShader("fragmentShaders.glsl");

    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    mProgram->link();
    if (!mProgram->isLinked()) {
        qDebug() << "Shader program failed to link.";
        // Handle the linking failure appropriately
    }

    // Fetch uniform locations after linking the program
    m_matrixUniform = mProgram->uniformLocation("matrix");
    m_lightPositionUniform = mProgram->uniformLocation("lightPosition");

    if (m_matrixUniform == -1 || m_lightPositionUniform == -1) {
        qDebug() << "Failed to fetch uniform location(s).";
        // Handle the error appropriately
    }

    // Set the light position as a uniform (you can update this as needed)
    QVector4D lightPos(1.0f, 1.0f, 1.0f, 0.0f); // Positional light
    mProgram->setUniformValue(m_lightPositionUniform, lightPos);

    m_posAttr = mProgram->attributeLocation("posAttr");
    if (m_posAttr == -1 || m_colAttr == -1 || m_matrixUniform == -1)
    {
        qDebug() << "Shader attribute or uniform location error.";
        // Handle the error appropriately, e.g., return or throw an exception
    }
    if (m_matrixUniform == -1 || m_lightPositionUniform == -1) {
        qDebug() << "Failed to fetch uniform location(s).";
        // Handle the error appropriately
    }
    
    mProgram->link();
    if (!mProgram->isLinked()) {
        qDebug() << "Shader program failed to link.";
        // Handle the linking failure appropriately
    }
}

void OpenGLWindow::processTriangles(const std::vector<Triangle>& triangles)
{
    // Clear existing voxel data
    mVertices.clear();
    mColors.clear();

    // Process the triangles and update voxel data
    for (const auto& triangle : triangles)
    {
        // Assuming you have functions in Triangle to retrieve vertices and normal
        Point3D normal = triangle.normal();
        Point3D p1 = triangle.p1();
        Point3D p2 = triangle.p2();
        Point3D p3 = triangle.p3();

        // Add your logic to update mVertices and mColors based on the triangle data
        // ...
        mVertices << p1.x() * 0.1 << p1.y() * 0.1 << p1.z() * 0.1;
        mColors << 1.0f << 0.0f << 0.0f;
        mVertices << p2.x() * 0.1 << p2.y() * 0.1 << p2.z() * 0.1;
        mColors << 1.0f << 0.0f << 0.0f;
        mVertices << p3.x() * 0.1 << p3.y() * 0.1 << p3.z() * 0.1;
        mColors << 1.0f << 0.0f << 0.0f;
        /*mVertices << p1.x() * 0.1 << p1.y() * 0.1 << p1.z() * 0.1;
        mColors << 1.0f << 1.0f << 1.0f;*/
        mNormals << normal.x() << normal.y() << normal.z();
       
    }

    // Notify that the shape has been updated
    emit shapeUpdate();
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f * dx);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.5f * dy);

        rotationAngle = rotX * rotY * rotationAngle;
        update();
    }
    lastPos = event->pos();
}

void OpenGLWindow::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();

    // Adjust the zoom factor based on the mouse wheel movement
    if (delta > 0) {
        // Zoom in
        zoomFactor *= 1.1f;
    }
    else {
        // Zoom out
        zoomFactor /= 1.1f;
    }

    update();
}
void OpenGLWindow::updateData(const QVector<GLfloat>& vertices, const QVector<GLfloat>& colors)
{
    mVertices = vertices;
    mColors = colors;
    update();
}
QString OpenGLWindow::readShader(QString filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Invalid file";
    QTextStream stream(&file);
    QString line = stream.readLine();
    while (!stream.atEnd())
    {
        line.append(stream.readLine());
    }
    return line;
}void OpenGLWindow::drawPlane() {
    // Define vertices for a simple plane
    QVector<GLfloat> planeVertices = {
        -5.0f, 0.0f, -5.0f,   // Vertex 1
         5.0f, 0.0f, -5.0f,   // Vertex 2
         5.0f, 0.0f,  5.0f,   // Vertex 3
        -5.0f, 0.0f,  5.0f    // Vertex 4
    };

    // Define indices for the plane
    QVector<GLuint> planeIndices = {
        0, 1, 2,    // Triangle 1
        0, 2, 3     // Triangle 2
    };

    // Vertex Array Object (VAO) setup
    QOpenGLVertexArrayObject planeVAO;
    planeVAO.create();
    planeVAO.bind();

    // Vertex Buffer Object (VBO) setup
    QOpenGLBuffer planeVBO(QOpenGLBuffer::VertexBuffer);
    planeVBO.create();
    planeVBO.bind();
    planeVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    planeVBO.allocate(planeVertices.constData(), planeVertices.size() * sizeof(GLfloat));

    // Enable vertex attributes
    glEnableVertexAttribArray(m_posAttr);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Index Buffer Object (IBO) setup
    QOpenGLBuffer planeIBO(QOpenGLBuffer::IndexBuffer);
    planeIBO.create();
    planeIBO.bind();
    planeIBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    planeIBO.allocate(planeIndices.constData(), planeIndices.size() * sizeof(GLuint));
    planeIBO.release();

    // Draw the plane
    glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, nullptr);

    // Clean up
    planeIBO.release();
    planeVBO.release();
    planeVAO.release();
}