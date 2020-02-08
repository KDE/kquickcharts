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
    QSGMaterialShader *createShader() const override;

    QColor lineColor() const;
    QColor fillColor() const;
    float lineWidth() const;
    float aspect() const;
    QVector<QVector2D> points() const;
    QVector2D bounds() const;

    void setLineColor(const QColor &color);
    void setFillColor(const QColor &color);
    void setLineWidth(float width);
    void setPoints(const QVector<QVector2D> &points);
    void setAspect(float aspect);
    void setBounds(float min, float max);

private:
    QColor m_lineColor;
    QColor m_fillColor;
    float m_lineWidth = 0.0;
    float m_aspect = 1.0;
    QVector2D m_bounds;
    QVector<QVector2D> m_points;
};

class LineChartShader : public SDFShader
{
public:
    LineChartShader();
    ~LineChartShader();

    char const *const *attributeNames() const override;

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    int m_matrixLocation = 0;
    int m_opacityLocation = 0;
    int m_lineColorLocation = 0;
    int m_fillColorLocation = 0;
    int m_lineWidthLocation = 0;
    int m_pointsLocation = 0;
    int m_pointCountLocation = 0;
    int m_aspectLocation = 0;
    int m_boundsLocation = 0;
};

#endif // LINECHARTMATERIAL_H
