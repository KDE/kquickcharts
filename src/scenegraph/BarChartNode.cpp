/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "BarChartNode.h"

#include <QColor>
#include <QDebug>
#include <QSGVertexColorMaterial>

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

void BarChartNode::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
}

void BarChartNode::setValues(const QVector<QPair<QVector2D, QColor>> &values)
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
    for (const auto &entry : qAsConst(m_values)) {
        auto value = entry.first;
        value.setY(std::min(value.y() * m_rect.height(), m_rect.height()));
        auto color = entry.second;
        auto rect = QRectF{QPointF{value.x(), m_rect.bottom() - value.y()}, QSizeF{m_barWidth, value.y()}};
        bar(vertices, indices, index, rect, color);
    }

    m_geometry->markVertexDataDirty();
    m_geometry->markIndexDataDirty();
    markDirty(QSGNode::DirtyGeometry);
}

void BarChartNode::bar(QSGGeometry::ColoredPoint2D *vertices, quint16 *indices, int &index, const QRectF &bar, const QColor &color)
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
