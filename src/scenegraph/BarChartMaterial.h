/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QColor>
#include <QSGMaterial>
#include <QSGMaterialShader>

#include "SDFShader.h"

class BarChartMaterial : public QSGMaterial
{
public:
    BarChartMaterial();
    ~BarChartMaterial();

    QSGMaterialType *type() const override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QSGMaterialShader *createShader() const override;
#else
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override;
#endif
    int compare(const QSGMaterial *other) const override;

    QVector2D aspect = QVector2D{1.0, 1.0};
    float radius = 0.0;
    QColor backgroundColor = Qt::transparent;
};

class BarChartShader : public SDFShader
{
public:
    BarChartShader();
    ~BarChartShader();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    char const *const *attributeNames() const override;

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    int m_matrixLocation = 0;
    int m_opacityLocation = 0;
    int m_aspectLocation = 0;
    int m_backgroundColorLocation = 0;
    int m_radiusLocation = 0;
#else
    bool updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
#endif
};

#endif // PIECHARTMATERIAL_H
