/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
