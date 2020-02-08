/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BARCHARTNODE_H
#define BARCHARTNODE_H

#include <QSGGeometryNode>

/**
 * @todo write docs
 */
class BarChartNode : public QSGGeometryNode
{
public:
    /**
     * Default constructor
     */
    BarChartNode();

    /**
     * Destructor
     */
    ~BarChartNode();

    void setRect(const QRectF &rect);
    void setValues(const QVector<QPair<QVector2D, QColor>> &values);
    void setBarWidth(qreal width);
    void update();

private:
    void bar(QSGGeometry::ColoredPoint2D *vertices, quint16 *indices, int &index, const QRectF &bar, const QColor &color);

    QRectF m_rect;
    QVector<QPair<QVector2D, QColor>> m_values;
    qreal m_barWidth = 0.0;
    QSGGeometry *m_geometry = nullptr;
};

#endif // BARCHARTNODE_H
