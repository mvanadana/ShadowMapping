#include "stdafx.h"
#include "Texture2D.h"
#include <QImage>
#include <QDebug>

Texture2D::Texture2D() 
    :mTexture(QOpenGLTexture::Target2D)
{
}

Texture2D::~Texture2D() {
    mTexture.destroy();
}

bool Texture2D::loadTexture(const QString& fileName, bool generateMipMaps) {
    QImage image(fileName);
    if (image.isNull()) {
        qDebug() << "Error loading texture '" << fileName << "'";
        return false;
    }

    image = image.convertToFormat(QImage::Format_RGBA8888);

    mTexture.setData(image.mirrored(false, true)); // Vertical flip for OpenGL textures

    if (generateMipMaps) {
        mTexture.generateMipMaps();
    }

    return true;
}

void Texture2D::bind(GLuint texUnit) {
    mTexture.bind(texUnit);
}

void Texture2D::unbind(GLuint texUnit) {
    Q_UNUSED(texUnit);
    mTexture.release();
}
