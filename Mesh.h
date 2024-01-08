#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector2D>
#include <QString>

struct Vertex {
    QVector3D position;
    QVector2D texCoords;
};

class Mesh : protected QOpenGLFunctions {
public:
    Mesh();
    ~Mesh();

    bool loadOBJ(const QString& filename);
    void draw();

private:
    bool mLoaded;
    QVector<Vertex> mVertices;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;

    void initBuffers();
};
