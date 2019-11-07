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

#ifndef LINECHARTNODE_H
#define LINECHARTNODE_H

#include <QColor>
#include <QSGNode>

class QRectF;
class LineChartMaterial;
class LineSegmentNode;

/**
 * @todo write docs
 */
class LineChartNode : public QSGNode
{
public:
    LineChartNode();

    /**
     * Destructor
     */
    ~LineChartNode();

    void setRect(const QRectF &rect);
    void setLineWidth(float width);
    void setLineColor(const QColor &color);
    void setFillColor(const QColor &color);
    void setValues(const QVector<QVector2D> &values);

private:
    void updatePoints();

    QRectF m_rect;
    float m_lineWidth = 0.0;
    float m_aspect = 1.0;
    QColor m_lineColor;
    QColor m_fillColor;
    QVector<QVector2D> m_values;
    QVector<LineSegmentNode *> m_segments;
};

#endif // LINECHARTNODE_H
