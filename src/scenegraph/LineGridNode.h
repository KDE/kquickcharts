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

#ifndef LINEGRIDNODE_H
#define LINEGRIDNODE_H

#include <QColor>
#include <QSGGeometryNode>

class QSGFlatColorMaterial;

/**
 * @todo write docs
 */
class LineGridNode : public QSGGeometryNode
{
public:
    LineGridNode();
    ~LineGridNode();

    void setVisible(bool visible);
    void setVertical(bool vertical);
    void setRect(const QRectF &rect);
    void setColor(const QColor &color);
    void setSpacing(float spacing);
    void setLineWidth(float lineWidth);

    bool isSubtreeBlocked() const override;

    void update();

private:
    void line(QSGGeometry::Point2D *vertices, quint16 *indices, int &index, qreal fromX, qreal fromY, qreal toX, qreal toY);

    QSGGeometry *m_geometry = nullptr;
    QSGFlatColorMaterial *m_material = nullptr;

    bool m_visible = true;
    bool m_vertical = false;
    QRectF m_rect;
    float m_spacing = 1.0;
    float m_lineWidth = 1.0;
};

#endif // LINEGRIDNODE_H
