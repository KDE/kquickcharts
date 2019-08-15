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

#ifndef BARCHART_H
#define BARCHART_H

#include "XYChart.h"

BEGIN_NAMESPACE

/**
 * A bar chart.
 */
class BarChart : public XYChart
{
    Q_OBJECT
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(qreal barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)

public:
    enum WidthMode { AutoWidth = -2 };
    Q_ENUM(WidthMode)

    BarChart(QQuickItem* parent = nullptr);
    ~BarChart();

    qreal spacing() const;
    Q_SLOT void setSpacing(qreal newSpacing);
    Q_SIGNAL void spacingChanged();

    qreal barWidth() const;
    Q_SLOT void setBarWidth(qreal newBarWidth);
    Q_SIGNAL void barWidthChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *) override;
    void onDataChanged() override;

private:
    qreal m_spacing = 0.0;
    qreal m_barWidth = AutoWidth;
    QVector<QVector<QPair<qreal, QColor>>> m_values;
};

END_NAMESPACE

#endif // BARCHART_H
