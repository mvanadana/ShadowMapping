#include "stdafx.h"
#include "Mesh.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Mesh::Mesh() : mLoaded(false) {
}

Mesh::~Mesh() {
    mVAO.destroy();
    mVBO.destroy();
}

bool Mesh::loadOBJ(const QString& filename) {
    if (!filename.endsWith(".obj", Qt::CaseInsensitive)) {
        qDebug() << "Unsupported file format. Only .obj files are supported.";
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open " << filename;
        return false;
    }

    QTextStream in(&file);
    QVector<QVector3D> tempVertices;
    QVector<QVector2D> tempUVs;
    QVector<unsigned int> vertexIndices, uvIndices;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);

        if (tokens.isEmpty())
            continue;

        if (tokens[0] == "v") {
            // Vertex position
            QVector3D vertex(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat());
            tempVertices.append(vertex);
        }
        else if (tokens[0] == "vt") {
            // Texture coordinates
            QVector2D uv(tokens[1].toFloat(), tokens[2].toFloat());
            tempUVs.append(uv);
        }
        else if (tokens[0] == "f") {
            // Face indices
            for (int i = 1; i <= 3; ++i) {
                QStringList faceTokens = tokens[i].split('/', Qt::SkipEmptyParts);
                if (faceTokens.size() >= 2) {
                    vertexIndices.append(faceTokens[0].toUInt());
                    uvIndices.append(faceTokens[1].toUInt());
                }
            }
        }
    }

    file.close();

    // Construct mesh vertices from indices
    for (int i = 0; i < vertexIndices.size(); ++i) {
        Vertex meshVertex;
        meshVertex.position = tempVertices[vertexIndices[i] - 1];
        meshVertex.texCoords = tempUVs[uvIndices[i] - 1];
        mVertices.append(meshVertex);
    }

    initBuffers();
    return (mLoaded = true);
}

void Mesh::initBuffers() {
    initializeOpenGLFunctions();

    mVAO.create();
    mVAO.bind();

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertices.constData(), mVertices.size() * sizeof(Vertex));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(QVector3D)));

    mVBO.release();
    mVAO.release();
}

void Mesh::draw() {
    if (!mLoaded)
        return;

    mVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    mVAO.release();
}

