#include "stdafx.h"
#include "Visualizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3); 
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);*/
    Visualizer w;
    w.show();
    return a.exec();
}
