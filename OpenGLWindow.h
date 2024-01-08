// OpenGLWindow.h
#pragma once
#include "Triangle.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>



class QOpenGLTexture;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLPaintDevice;

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
signals:
    void shapeUpdate();

public:
    OpenGLWindow(const QColor& background, QMainWindow* parent);
    ~OpenGLWindow();

    void mouseMoveEvent(QMouseEvent* event);
    void processTriangles(const std::vector<Triangle>& triangles);
    void wheelEvent(QWheelEvent* event);
    void updateData(const QVector<GLfloat>& vertices, const QVector<GLfloat>& colors);
    QString readShader(QString filepath);

protected:
    void paintGL() override;
    void initializeGL() override;
   
    //void paintEvent(QPaintEvent* event) override;
    

private:
    void reset();
    void drawPlane();

private:
    
    QOpenGLShader* mVshader = nullptr;
    QOpenGLShader* mFshader = nullptr;
    QOpenGLShaderProgram* mProgram = nullptr;
    QOpenGLBuffer mVbo;
    
    int mVertexAttr;
    int mNormalAttr;
    int mMatrixUniform;
    int m_lightPositionUniform;
    QColor mBackground;
    QMetaObject::Connection mContextWatchConnection;
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    GLint m_normalAttr;
    QVector<QVector2D> mPixelVertices;

    QVector<GLfloat> mVertices;
    QVector<GLfloat> mColors;
    QVector<GLfloat> mNormals;

    std::vector<float> voxelData;  // New variable to store voxel data
    int gridSize = 12;
    float zoomFactor = 1.0f;

    QQuaternion rotationAngle;
    QPoint lastPos;
   
public slots:
    //void displaySTL();  // New slot to trigger voxel display
};
