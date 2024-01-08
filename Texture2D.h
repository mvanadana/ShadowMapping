#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <QOpenGLFunctions>
#include <QImage>
#include <QOpenGLTexture>
#include <QString>

class Texture2D : protected QOpenGLFunctions {
public:
    Texture2D();
    ~Texture2D();

    bool loadTexture(const QString& fileName, bool generateMipMaps = true);
    void bind(GLuint texUnit = 0);
    void unbind(GLuint texUnit = 0);

private:
    QOpenGLTexture mTexture;
};

#endif // TEXTURE2D_H
