#pragma once

#include <QtWidgets/QMainWindow>
//#include "Visualize.h"
//#include "OpenGLWindow.h"
//#include "SymmetricDDA.h"
#include <Qvector>
#include <bitset>
#include <string>
#include <cstdint> // Include this header for uint16_t and uint32_t

struct TriangleInfo {
    float normal[3];
    float vertex1[3];
    float vertex2[3];
    float vertex3[3];
    uint16_t attributeByteCount;
};

class OpenGLWindow;

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer(QWindow* parent = nullptr);
    ~Visualizer();

private:
    void setupUi();

private:
    OpenGLWindow* mRenderer;

    QPushButton* mUploadButton;
    QPushButton* mDisplayButton;
    QVector<GLfloat>mVertices;
    QVector<GLfloat>mColors;
    std::string mFileName;
    

private:

    void openFileDialog();
    void displayButtonClicked();
    void readSTL();
    void readVertex(std::string filePath);
    void readColors(std::string filePath);


    
};
