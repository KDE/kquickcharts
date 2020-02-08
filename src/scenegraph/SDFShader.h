/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef SDF_SHADER_H
#define SDF_SHADER_H

#include <QSGMaterialShader>

class SDFShader : public QSGMaterialShader
{
public:
    SDFShader();
    virtual ~SDFShader();

    void setShaders(const QString &vertex, const QString &fragment);
};

#endif // SDF_SHADER_H
