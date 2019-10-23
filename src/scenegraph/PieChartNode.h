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

#ifndef PIECHARTNODE_H
#define PIECHARTNODE_H

#include <QColor>
#include <QSGGeometryNode>

class QRectF;
class PieChartMaterial;

/**
 * @todo write docs
 */
class PieChartNode : public QSGGeometryNode
{
public:
    PieChartNode();

    /**
     * Default constructor
     */
    explicit PieChartNode(const QRectF &rect);

    /**
     * Destructor
     */
    ~PieChartNode();

    void setRect(const QRectF &rect);
    void setInnerRadius(qreal radius);
    void setOuterRadius(qreal radius);
    void setSections(const QVector<qreal> &sections);
    void setColors(const QVector<QColor> &colors);
    void setBackgroundColor(const QColor &color);
    void setFromAngle(qreal angle);
    void setToAngle(qreal angle);
    void setSmoothEnds(bool smooth);

private:
    void updateTriangles();

    QRectF m_rect;
    qreal m_innerRadius = 0.0;
    qreal m_outerRadius = 0.0;
    QColor m_backgroundColor;
    qreal m_fromAngle = 0.0;
    qreal m_toAngle = 360.0;
    bool m_smoothEnds = false;

    QVector<qreal> m_sections;
    QVector<QColor> m_colors;

    QSGGeometry *m_geometry = nullptr;
    PieChartMaterial *m_material = nullptr;
};

#endif // PIECHARTNODE_H
