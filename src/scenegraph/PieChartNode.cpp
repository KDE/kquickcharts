/*
 * This file is part of Quick Charts
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

#include "PieChartNode.h"

#include <algorithm>

#include <QColor>
#include <QSGGeometry>
#include <cmath>

#include "PieChartMaterial.h"

static const qreal pi = std::acos(-1.0);
static const qreal sectionSize = pi * 0.5;

inline QVector4D colorToVec4(const QColor &color)
{
    return QVector4D{float(color.redF()), float(color.greenF()), float(color.blueF()), float(color.alphaF())};
}

inline qreal degToRad(qreal deg)
{
    return (deg / 180.0) * pi;
}

inline QVector2D rotated(const QVector2D vector, qreal angle)
{
    auto newX = vector.x() * std::cos(angle) - vector.y() * std::sin(angle);
    auto newY = vector.x() * std::sin(angle) + vector.y() * std::cos(angle);
    return QVector2D(newX, newY);
}

PieChartNode::PieChartNode()
    : PieChartNode(QRectF{})
{
}

PieChartNode::PieChartNode(const QRectF &rect)
{
    m_geometry = new QSGGeometry{QSGGeometry::defaultAttributes_TexturedPoint2D(), 4};
    QSGGeometry::updateTexturedRectGeometry(m_geometry, rect, QRectF{0, 0, 1, 1});
    setGeometry(m_geometry);

    m_material = new PieChartMaterial{};
    setMaterial(m_material);

    setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

PieChartNode::~PieChartNode()
{
}

void PieChartNode::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
    QSGGeometry::updateTexturedRectGeometry(m_geometry, m_rect, QRectF{0, 0, 1, 1});
    markDirty(QSGNode::DirtyGeometry);

    auto minDimension = qMin(m_rect.width(), m_rect.height());

    QVector2D aspect{1.0, 1.0};
    aspect.setX(rect.width() / minDimension);
    aspect.setY(rect.height() / minDimension);
    m_material->setAspectRatio(aspect);

    m_material->setInnerRadius(m_innerRadius / minDimension);
    m_material->setOuterRadius(m_outerRadius / minDimension);

    markDirty(QSGNode::DirtyMaterial);
}

void PieChartNode::setInnerRadius(qreal radius)
{
    if (qFuzzyCompare(radius, m_innerRadius)) {
        return;
    }

    m_innerRadius = radius;

    auto minDimension = qMin(m_rect.width(), m_rect.height());
    m_material->setInnerRadius(m_innerRadius / minDimension);

    markDirty(QSGNode::DirtyMaterial);
}

void PieChartNode::setOuterRadius(qreal radius)
{
    if (qFuzzyCompare(radius, m_outerRadius)) {
        return;
    }

    m_outerRadius = radius;

    auto minDimension = qMin(m_rect.width(), m_rect.height());
    m_material->setOuterRadius(m_outerRadius / minDimension);

    markDirty(QSGNode::DirtyMaterial);
}

void PieChartNode::setColors(const QVector<QColor> &colors)
{
    m_colors = colors;
    updateTriangles();
}

void PieChartNode::setSections(const QVector<qreal> &sections)
{
    m_sections = sections;
    updateTriangles();
}

void PieChartNode::setBackgroundColor(const QColor &color)
{
    if (color == m_backgroundColor)
        return;

    m_backgroundColor = color;
    if (qFuzzyCompare(m_toAngle, 360.0)) {
        m_material->setBackgroundColor(color);
        markDirty(QSGNode::DirtyMaterial);
    } else {
        updateTriangles();
    }
}

void PieChartNode::setFromAngle(qreal angle)
{
    if (qFuzzyCompare(angle, m_fromAngle)) {
        return;
    }

    m_fromAngle = angle;
    updateTriangles();
}

void PieChartNode::setToAngle(qreal angle)
{
    if (qFuzzyCompare(angle, m_fromAngle)) {
        return;
    }

    m_toAngle = angle;

    if (!qFuzzyCompare(m_toAngle, 360.0)) {
        m_material->setBackgroundColor(Qt::transparent);
    } else {
        m_material->setBackgroundColor(m_backgroundColor);
    }

    updateTriangles();
}

void PieChartNode::setSmoothEnds(bool smooth)
{
    if (smooth == m_smoothEnds) {
        return;
    }

    m_smoothEnds = smooth;
    m_material->setSmoothEnds(smooth);
    markDirty(QSGNode::DirtyMaterial);
}

void PieChartNode::updateTriangles()
{
    if (m_sections.isEmpty() || m_sections.size() != m_colors.size())
        return;

    QVector<QVector2D> trianglePoints;
    QVector<QVector4D> triangleColors;
    QVector<int> segments;

    qreal totalAngle = degToRad(m_toAngle);
    qreal overlap = m_smoothEnds ? 0.2 : 0.05;

    auto sections = m_sections;
    auto colors = m_colors;

    QVector2D point = rotated(QVector2D{0.0, -2.0}, degToRad(m_fromAngle));
    auto index = 0;
    auto current = sections.at(0) * totalAngle;
    auto sectionCount = 0;
    auto total = 0.0;

    while (index < sections.size()) {
        auto currentSection = std::max(current - sectionSize, 0.0);
        auto angle = (currentSection > 0.0) ? sectionSize : current;
        auto overlapAngle = angle + (m_smoothEnds ? overlap : std::min(currentSection, overlap));
        overlapAngle = index == sections.size() - 1 && currentSection <= 0.0 ? angle : overlapAngle;

        trianglePoints << point;
        trianglePoints << rotated(point, overlapAngle);

        point = rotated(point, angle);
        current -= angle;
        sectionCount++;

        while (qFuzzyCompare(current, 0.0)) {
            triangleColors << colorToVec4(colors.at(index));
            segments << sectionCount;
            sectionCount = 0;
            total += sections.at(index);
            index++;

            if (index < sections.size()) {
                current = sections.at(index) * totalAngle;
            } else {
                break;
            }
        }
    }

    if (sections.size() == 1 && qFuzzyCompare(sections.at(0), 0.0)) {
        trianglePoints.clear();
        triangleColors.clear();
        segments.clear();
    }

    if (!qFuzzyCompare(totalAngle, 360.0) && total < 1.0) {
        sectionCount = 0;
        current = (1.0 - total) * totalAngle;

        auto overlapAngle = std::min(total * totalAngle, overlap);
        point = rotated(point, -overlapAngle);
        current += overlapAngle;

        while (current > 0.0) {
            auto currentSection = std::max(current - sectionSize, 0.0);
            auto angle = (currentSection > 0.0) ? sectionSize : current;

            trianglePoints.prepend(point);
            trianglePoints.prepend(rotated(point, angle + std::min(currentSection, overlap)));

            point = rotated(point, angle);
            current -= angle;
            sectionCount++;
        }

        triangleColors.prepend(colorToVec4(m_backgroundColor));
        segments.prepend(sectionCount);
    }

    m_material->setTriangles(trianglePoints);
    m_material->setColors(triangleColors);
    m_material->setSegments(segments);

    markDirty(QSGNode::DirtyMaterial);
}
