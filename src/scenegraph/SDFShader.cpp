#include "SDFShader.h"

#include <QOpenGLContext>

static const QString shaderRoot = QStringLiteral(":/org.kde.quickcharts/");

SDFShader::SDFShader()
{

}

SDFShader::~SDFShader()
{
}

void SDFShader::setShaders(const QString& vertex, const QString& fragment)
{
    auto header = QOpenGLContext::currentContext()->isOpenGLES() ? QStringLiteral("es_header.glsl") : QStringLiteral("desktop_header.glsl");

    setShaderSourceFiles(QOpenGLShader::Vertex, {
        shaderRoot + header,
        shaderRoot + vertex
    });

    setShaderSourceFiles(QOpenGLShader::Fragment, {
        shaderRoot + header,
        shaderRoot + QStringLiteral("sdf.frag"),
        shaderRoot + fragment
    });
}
