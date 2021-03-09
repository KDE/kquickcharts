/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "SDFShader.h"

#include <QOpenGLContext>
#include <QSurfaceFormat>

static const char shaderRoot[] = ":/org.kde.quickcharts/";

SDFShader::SDFShader()
{
}

SDFShader::~SDFShader()
{
}

void SDFShader::setShaders(const QString &vertex, const QString &fragment)
{
    auto header = QStringLiteral("header_desktop.glsl");
    auto format = QOpenGLContext::currentContext()->format();
    if (format.renderableType() == QSurfaceFormat::OpenGLES) {
        header = QStringLiteral("header_es2.glsl");
    }

    setShaderSourceFiles(QOpenGLShader::Vertex, {QString::fromLatin1(shaderRoot) + header, QString::fromLatin1(shaderRoot) + vertex});

    setShaderSourceFiles(QOpenGLShader::Fragment,
                         {QString::fromLatin1(shaderRoot) + header, //
                          QString::fromLatin1(shaderRoot) + QStringLiteral("sdf.glsl"), //
                          QString::fromLatin1(shaderRoot) + fragment});
}
