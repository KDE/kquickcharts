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

#include "BarChartNode.h"

#include <QSGVertexColorMaterial>
#include <QColor>
#include <QDebug>

bool compareCount(const QVector<QPair<qreal, QColor>> &first, const QVector<QPair<qreal, QColor>> &second)
{
    return first.count() < second.count();
}

BarChartNode::BarChartNode()
{
    m_geometry = new QSGGeometry{QSGGeometry::defaultAttributes_ColoredPoint2D(), 0};
    m_geometry->setDrawingMode(QSGGeometry::DrawTriangles);
    setGeometry(m_geometry);

    setMaterial(new QSGVertexColorMaterial{});

    setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

BarChartNode::~BarChartNode()
{

}

void BarChartNode::setRect(const QRectF& rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
}

void BarChartNode::setValues(const QVector<QPair<QVector2D, QColor>>& values)
{
    m_values = values;
}

void BarChartNode::setBarWidth(qreal width)
{
    if (qFuzzyCompare(width, m_barWidth))
        return;

    m_barWidth = width;
}

void BarChartNode::update()
{
    if (!m_rect.isValid() || m_values.isEmpty())
        return;

    auto itemCount = m_values.count();

    if (itemCount <= 0)
        return;

    int totalVertices = itemCount * 6;
    if (totalVertices != m_geometry->vertexCount()) {
        m_geometry->allocate(totalVertices, totalVertices);
    }

    auto vertices = m_geometry->vertexDataAsColoredPoint2D();
    auto indices = m_geometry->indexDataAsUShort();

    auto index = 0;
    for (auto entry : qAsConst(m_values)) {
        auto value = entry.first;
        value.setY(value.y() * m_rect.height());
        auto color = entry.second;
        auto rect = QRectF{QPointF{value.x(), m_rect.bottom() - value.y()}, QSizeF{m_barWidth, value.y()}};
        bar(vertices, indices, index, rect, color);
    }

    m_geometry->markVertexDataDirty();
    m_geometry->markIndexDataDirty();
    markDirty(QSGNode::DirtyGeometry);
}

void BarChartNode::bar(QSGGeometry::ColoredPoint2D* vertices, quint16* indices, int& index, const QRectF& bar, const QColor& color)
{
    indices[index] = index;
    vertices[index++].set(bar.left(), bar.bottom(), color.red(), color.green(), color.blue(), color.alpha());
    indices[index] = index;
    vertices[index++].set(bar.left(), bar.top(), color.red(), color.green(), color.blue(), color.alpha());
    indices[index] = index;
    vertices[index++].set(bar.right(), bar.top(), color.red(), color.green(), color.blue(), color.alpha());
    indices[index] = index;
    vertices[index++].set(bar.right(), bar.top(), color.red(), color.green(), color.blue(), color.alpha());
    indices[index] = index;
    vertices[index++].set(bar.right(), bar.bottom(), color.red(), color.green(), color.blue(), color.alpha());
    indices[index] = index;
    vertices[index++].set(bar.left(), bar.bottom(), color.red(), color.green(), color.blue(), color.alpha());
}
