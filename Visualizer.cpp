#include "stdafx.h"
#include "Visualizer.h"
#include "ReadSTL.h"
#include "OpenGLWindow.h"
#include<string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

Visualizer::Visualizer(QWindow* parent)
{
    setupUi();
    // Connect the buttons to their respective slots
    connect(mUploadButton, &QPushButton::clicked, this, &Visualizer::openFileDialog);
    connect(mDisplayButton, &QPushButton::clicked, this, &Visualizer::readSTL);
}

Visualizer::~Visualizer()
{
}

void Visualizer::setupUi()
{
    resize(1530, 785);

    setWindowTitle(QCoreApplication::translate("Shadow simulation", nullptr));

    mUploadButton = new QPushButton(this);
    mUploadButton->setObjectName("uploadButton");
    mUploadButton->setText("Upload STL");
    mUploadButton->setGeometry(QRect(1300, 50, 200, 30));

    mDisplayButton = new QPushButton(this);
    mDisplayButton->setObjectName("displayButton");
    mDisplayButton->setText("Display");
    mDisplayButton->setGeometry(QRect(1300, 100, 200, 30));

    mRenderer = new OpenGLWindow(Qt::white, this);
    //setCentralWidget(mRenderer);

}

void Visualizer::openFileDialog()
{
    QString qFileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "", tr("STL Files (*.stl);;All Files (*)"));

    if (!qFileName.isEmpty()) {
        std::string fileName = qFileName.toStdString();

        // Use the Reader class to read the STL file and create Triangle objects
        ReadSTL reader;
        std::vector<Triangle> triangles;
      
        reader.read(fileName, triangles);

        // Now, you can process the triangles as needed. 
        // For example, pass them to OpenGLWindow
        mRenderer->processTriangles(triangles);

        // emit signal to notify OpenGLWindow about the new file
        emit mRenderer->shapeUpdate();
    }
}


void Visualizer::displayButtonClicked()
{
    // Implement the code to trigger the display action in OpenGLWindow
    // For example: mOpenGLWindow->displaySTL();
}
void Visualizer::readSTL()
{

    QString qFileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "", tr("STL Files (*.stl);;All Files (*)"));

    if (!qFileName.isEmpty()) {
        std::string fileName = qFileName.toStdString();

        // Use the Reader class to read the STL file and create Triangle objects
        std::string filePath = fileName;
        readVertex(filePath);
        readColors(filePath);
        mRenderer->updateData(mVertices, mColors);

        mVertices.clear();
        mColors.clear();

    }
}

    void Visualizer::readVertex(std::string filePath)
    {
        std::ifstream file(filePath, std::ios::binary);

        if (file.is_open())
        {
            // Read the header (80 bytes, usually skipped)
            char header[80];
            file.read(header, 80);

            // Read the number of triangles (4 bytes)
            uint32_t numTriangles;
            file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));

            // Read each triangle
            for (uint32_t i = 0; i < numTriangles; ++i)
            {
                TriangleInfo triangle;
                file.read(reinterpret_cast<char*>(&triangle), sizeof(triangle) - 2);
                mVertices << triangle.vertex1[0] * 0.1 << triangle.vertex1[1] * 0.1 << triangle.vertex1[2] * 0.1;
                mVertices << triangle.vertex2[0] * 0.1 << triangle.vertex2[1] * 0.1 << triangle.vertex2[2] * 0.1;
                mVertices << triangle.vertex3[0] * 0.1 << triangle.vertex3[1] * 0.1 << triangle.vertex3[2] * 0.1;
                mVertices << triangle.vertex1[0] * 0.1 << triangle.vertex1[1] * 0.1 << triangle.vertex1[2] * 0.1;
/*
                mColors << 1 << 1 << 1;
                mColors << 1 << 1 << 1;
                mColors << 1 << 1 << 1;
                mColors << 1 << 1 << 1;*/

            }
        }
        else
        {
            std::cerr << "Error opening file." << std::endl;
        }
    }
    void Visualizer::readColors(std::string filePath)
    {
        std::ifstream file(filePath, std::ios::binary);

        if (file.is_open())
        {
            // Read the header (80 bytes, usually skipped)
            char header[80];
            file.read(header, 80);

            // Read the number of triangles (4 bytes)
            uint32_t numTriangles;
            file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));
            for (uint32_t i = 0; i < numTriangles; ++i)
            {
                TriangleInfo triangle;
                file.read(reinterpret_cast<char*>(&triangle), sizeof(triangle));
                // Convert the 16-bit binary representation to three parts of 5 bits each
                std::bitset<16> binaryRepresentation(triangle.attributeByteCount);
                std::bitset<5> part1(binaryRepresentation.to_string().substr(0, 5));
                std::bitset<5> part2(binaryRepresentation.to_string().substr(5, 10));
                std::bitset<5> part3(binaryRepresentation.to_string().substr(10, 15));

                // Convert each part to decimal and divide by 31
                float decimalPart1 = part1.to_ulong() / 31.0f;
                float decimalPart2 = part2.to_ulong() / 31.0f;
                float decimalPart3 = part3.to_ulong() / 31.0f;


                mColors << decimalPart1 << decimalPart2 <<  decimalPart3;
                mColors <<  decimalPart1 << decimalPart2 <<  decimalPart3;
                mColors << decimalPart1 << decimalPart2 << decimalPart3;
                mColors <<  decimalPart1 << decimalPart2 << decimalPart3;

            }

        }
        else {
            std::cerr << "Error opening file." << std::endl;
        }

    }