/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LINECHARTMATERIAL_H
#define LINECHARTMATERIAL_H

#include <QColor>
#include <QSGMaterial>
#include <QSGMaterialShader>

#include "SDFShader.h"

class LineChartMaterial : public QSGMaterial
{
public:
    LineChartMaterial();
    ~LineChartMaterial();

    QSGMaterialType *type() const override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QSGMaterialShader *createShader() const override;
#else
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override;
#endif
    int compare(const QSGMaterial *other) const override;

    float aspect = 1.0;
    float lineWidth = 0.0;
    float smoothing = 0.1;
};

class LineChartShader : public SDFShader
{
public:
    LineChartShader();
    ~LineChartShader();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    char const *const *attributeNames() const override;

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    int m_matrixLocation = 0;
    int m_opacityLocation = 0;
    int m_lineWidthLocation = 0;
    int m_aspectLocation = 0;
    int m_smoothingLocation = 0;
#else
    bool updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
#endif
};

#endif // LINECHARTMATERIAL_H
