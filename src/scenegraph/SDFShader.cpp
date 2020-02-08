/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "SDFShader.h"

#include <QOpenGLContext>

static const char shaderRoot[] = ":/org.kde.quickcharts/";

SDFShader::SDFShader()
{
}

SDFShader::~SDFShader()
{
}

void SDFShader::setShaders(const QString &vertex, const QString &fragment)
{
    auto header = QOpenGLContext::currentContext()->isOpenGLES() ? QStringLiteral("es_header.glsl") : QStringLiteral("desktop_header.glsl");

    setShaderSourceFiles(QOpenGLShader::Vertex, {QString::fromLatin1(shaderRoot) + header, QString::fromLatin1(shaderRoot) + vertex});

    setShaderSourceFiles(
        QOpenGLShader::Fragment,
        {QString::fromLatin1(shaderRoot) + header,
         QString::fromLatin1(shaderRoot) + QStringLiteral("sdf.frag"),
         QString::fromLatin1(shaderRoot) + fragment});
}
