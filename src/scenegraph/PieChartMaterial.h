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

#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QSGMaterial>
#include <QSGMaterialShader>
#include <QColor>

#include "SDFShader.h"

class PieChartMaterial : public QSGMaterial
{
public:
    PieChartMaterial();
    ~PieChartMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    QVector2D aspectRatio() const;
    float innerRadius() const;
    float outerRadius() const;
    QColor backgroundColor() const;
    bool smoothEnds() const;

    QVector<QVector2D> triangles() const;
    QVector<QVector4D> colors() const;
    QVector<int> segments() const;

    void setAspectRatio(const QVector2D &aspect);
    void setInnerRadius(float radius);
    void setOuterRadius(float radius);
    void setBackgroundColor(const QColor &color);
    void setSmoothEnds(bool smooth);

    void setTriangles(const QVector<QVector2D> &triangles);
    void setColors(const QVector<QVector4D> &colors);
    void setSegments(const QVector<int> &segments);

private:
    QVector2D m_aspectRatio;
    float m_innerRadius = 0.0f;
    float m_outerRadius = 0.0f;
    QColor m_backgroundColor;
    bool m_smoothEnds = false;

    QVector<QVector2D> m_triangles;
    QVector<QVector4D> m_colors;
    QVector<int> m_segments;
};

class PieChartShader : public SDFShader
{
public:
    PieChartShader();
    ~PieChartShader();

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
    int m_trianglesLocation = 0;
    int m_colorsLocation = 0;
    int m_segmentsLocation = 0;
    int m_segmentCountLocation = 0;
    int m_smoothEndsLocation = 0;
};

#endif // PIECHARTMATERIAL_H
