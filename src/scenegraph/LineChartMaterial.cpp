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

#include "LineChartMaterial.h"

LineChartMaterial::LineChartMaterial()
{
    setFlag(QSGMaterial::Blending);
}

LineChartMaterial::~LineChartMaterial()
{
}

QSGMaterialType *LineChartMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *LineChartMaterial::createShader() const
{
    return new LineChartShader();
}

QColor LineChartMaterial::lineColor() const
{
    return m_lineColor;
}

QColor LineChartMaterial::fillColor() const
{
    return m_fillColor;
}

float LineChartMaterial::lineWidth() const
{
    return m_lineWidth;
}

QVector<QVector2D> LineChartMaterial::points() const
{
    return m_points;
}

float LineChartMaterial::aspect() const
{
    return m_aspect;
}

void LineChartMaterial::setLineColor(const QColor &color)
{
    m_lineColor = color;
}

void LineChartMaterial::setFillColor(const QColor &color)
{
    m_fillColor = color;
}

void LineChartMaterial::setLineWidth(float width)
{
    m_lineWidth = width;
}

void LineChartMaterial::setPoints(const QVector<QVector2D> &points)
{
    m_points = points;
}

void LineChartMaterial::setAspect(float aspect)
{
    m_aspect = aspect;
}

LineChartShader::LineChartShader()
{
    setShaders(QStringLiteral("linechart.vert"), QStringLiteral("linechart.frag"));
}

LineChartShader::~LineChartShader()
{
}

const char *const *LineChartShader::attributeNames() const
{
    static char const *const names[] = {"in_vertex", "in_uv", nullptr};
    return names;
}

void LineChartShader::initialize()
{
    QSGMaterialShader::initialize();
    m_matrixLocation = program()->uniformLocation("matrix");
    m_opacityLocation = program()->uniformLocation("opacity");
    m_lineColorLocation = program()->uniformLocation("lineColor");
    m_fillColorLocation = program()->uniformLocation("fillColor");
    m_lineWidthLocation = program()->uniformLocation("lineWidth");
    m_pointsLocation = program()->uniformLocation("points");
    m_pointCountLocation = program()->uniformLocation("pointCount");
    m_aspectLocation = program()->uniformLocation("aspect");
}

void LineChartShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrixLocation, state.combinedMatrix());
    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacityLocation, state.opacity());

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        LineChartMaterial *material = static_cast<LineChartMaterial *>(newMaterial);
        program()->setUniformValue(m_lineColorLocation, material->lineColor());
        program()->setUniformValue(m_fillColorLocation, material->fillColor());
        program()->setUniformValue(m_lineWidthLocation, material->lineWidth());
        program()->setUniformValue(m_aspectLocation, material->aspect());
        program()->setUniformValue(m_pointCountLocation, material->points().size());
        program()->setUniformValueArray(m_pointsLocation, material->points().constData(), material->points().size());
    }
}
