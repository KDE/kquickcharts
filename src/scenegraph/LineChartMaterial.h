/*
 * This file is part of Quick Charts.
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LINECHARTMATERIAL_H
#define LINECHARTMATERIAL_H

#include <QSGMaterial>
#include <QSGMaterialShader>
#include <QColor>

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

    void setLineColor(const QColor &color);
    void setFillColor(const QColor &color);
    void setLineWidth(float width);
    void setPoints(const QVector<QVector2D> &points);
    void setAspect(float aspect);

private:
    QColor m_lineColor;
    QColor m_fillColor;
    float m_lineWidth = 0.0;
    float m_aspect = 1.0;
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
};

#endif // LINECHARTMATERIAL_H
