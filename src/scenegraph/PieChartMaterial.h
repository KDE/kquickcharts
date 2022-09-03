/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QColor>
#include <QSGMaterial>
#include <QSGMaterialShader>

#include "SDFShader.h"

class PieChartMaterial : public QSGMaterial
{
public:
    PieChartMaterial();
    ~PieChartMaterial();

    QSGMaterialType *type() const override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QSGMaterialShader *createShader() const override;
#else
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override;
#endif

    QVector2D aspectRatio() const;
    float innerRadius() const;
    float outerRadius() const;
    QColor backgroundColor() const;
    bool smoothEnds() const;
    float fromAngle() const;
    float toAngle() const;

    QVector<QVector2D> segments() const;
    QVector<QVector4D> colors() const;

    void setAspectRatio(const QVector2D &aspect);
    void setInnerRadius(float radius);
    void setOuterRadius(float radius);
    void setBackgroundColor(const QColor &color);
    void setSmoothEnds(bool smooth);
    void setFromAngle(float angle);
    void setToAngle(float angle);

    void setSegments(const QVector<QVector2D> &triangles);
    void setColors(const QVector<QVector4D> &colors);

private:
    QVector2D m_aspectRatio;
    float m_innerRadius = 0.0f;
    float m_outerRadius = 0.0f;
    QColor m_backgroundColor;
    bool m_smoothEnds = false;
    float m_fromAngle = 0.0;
    float m_toAngle = 6.28318; // 2 * pi

    QVector<QVector2D> m_segments;
    QVector<QVector4D> m_colors;
};

class PieChartShader : public SDFShader
{
public:
    PieChartShader();
    ~PieChartShader();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    char const *const *attributeNames() const override;

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    int m_matrixLocation = 0;
    int m_opacityLocation = 0;
    int m_innerRadiusLocation = 0;
    int m_outerRadiusLocation = 0;
    int m_aspectLocation = 0;
    int m_backgroundColorLocation = 0;
    int m_colorsLocation = 0;
    int m_segmentsLocation = 0;
    int m_segmentCountLocation = 0;
    int m_smoothEndsLocation = 0;
    int m_fromAngleLocation = 0;
    int m_toAngleLocation = 0;
#else
    bool updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
#endif
};

#endif // PIECHARTMATERIAL_H
