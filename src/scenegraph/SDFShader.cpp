/*
 * This file is part of KQuickCharts
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <https://www.gnu.org/licenses/>.
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
