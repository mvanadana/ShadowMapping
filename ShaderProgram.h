#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QMap>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class ShaderProgram : protected QOpenGLFunctions {
public:
    ShaderProgram();
    ~ShaderProgram();

    enum ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    bool loadShaders(const QString& vsFilename, const QString& fsFilename);
    void use();

    GLuint getProgram() const;

    void setUniform(const char* name, const QVector2D& v);
    void setUniform(const char* name, const QVector3D& v);
    void setUniform(const char* name, const QVector4D& v);
    void setUniform(const char* name, const QMatrix4x4& m);

    GLint getUniformLocation(const char* name);

private:
    QString fileToString(const QString& filename);
    void checkCompileErrors(QOpenGLShader* shader, ShaderType type);

    QOpenGLShaderProgram mShaderProgram;
    QMap<QString, GLint> mUniformLocations;
};

#endif // SHADERPROGRAM_H
