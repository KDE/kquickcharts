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

#ifndef LINESEGMENTNODE_H
#define LINESEGMENTNODE_H

#include <QColor>
#include <QSGGeometryNode>
#include <QVector2D>

class QRectF;
class LineChartMaterial;

/**
 * @todo write docs
 */
class LineSegmentNode : public QSGGeometryNode
{
public:
    LineSegmentNode();

    /**
     * Default constructor
     */
    explicit LineSegmentNode(const QRectF &rect);

    /**
     * Destructor
     */
    ~LineSegmentNode();

    void setRect(const QRectF &rect);
    void setAspect(float xAspect, float yAspect);
    void setLineWidth(float width);
    void setLineColor(const QColor &color);
    void setFillColor(const QColor &color);
    void setValues(const QVector<QVector2D> &values);
    void setFarLeft(const QVector2D &value);
    void setFarRight(const QVector2D &value);

    void updatePoints();

private:
    QRectF m_rect;
    float m_lineWidth = 0.0;
    float m_xAspect = 1.0;
    float m_yAspect = 1.0;
    QVector2D m_farLeft;
    QVector2D m_farRight;
    QVector<QVector2D> m_values;
    QSGGeometry *m_geometry = nullptr;
    LineChartMaterial *m_material = nullptr;
};

#endif // LINESEGMENTNODE_H
