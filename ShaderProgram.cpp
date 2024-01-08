#include "stdafx.h"
#include "ShaderProgram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ShaderProgram::ShaderProgram() {
    initializeOpenGLFunctions();
}

ShaderProgram::~ShaderProgram() {
    mShaderProgram.removeAllShaders();
}

bool ShaderProgram::loadShaders(const QString& vsFilename, const QString& fsFilename) {
    if (!mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vsFilename)) {
        qDebug() << "Vertex shader compilation error:" << mShaderProgram.log();
        return false;
    }

    if (!mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fsFilename)) {
        qDebug() << "Fragment shader compilation error:" << mShaderProgram.log();
        return false;
    }

    if (!mShaderProgram.link()) {
        qDebug() << "Shader program linking error:" << mShaderProgram.log();
        return false;
    }

    return true;
}

void ShaderProgram::use() {
    mShaderProgram.bind();
}

GLuint ShaderProgram::getProgram() const {
    return mShaderProgram.programId();
}

void ShaderProgram::setUniform(const char* name, const QVector2D& v) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        mShaderProgram.setUniformValue(location, v);
    }
}

void ShaderProgram::setUniform(const char* name, const QVector3D& v) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        mShaderProgram.setUniformValue(location, v);
    }
}

void ShaderProgram::setUniform(const char* name, const QVector4D& v) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        mShaderProgram.setUniformValue(location, v);
    }
}

void ShaderProgram::setUniform(const char* name, const QMatrix4x4& m) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        mShaderProgram.setUniformValue(location, m);
    }
}

GLint ShaderProgram::getUniformLocation(const char* name) {
    if (mUniformLocations.contains(name)) {
        return mUniformLocations.value(name);
    }
    else {
        GLint location = mShaderProgram.uniformLocation(name);
        if (location == -1) {
            qDebug() << "Uniform" << name << "not found in the shader program.";
        }
        else {
            mUniformLocations.insert(name, location);
        }
        return location;
    }
}

QString ShaderProgram::fileToString(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file:" << filename;
        return "";
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return content;
}
