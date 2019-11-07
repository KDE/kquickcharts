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

#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include "XYChart.h"

class LineChartNode;

/**
 * A line chart.
 */
class LineChart : public XYChart
{
    Q_OBJECT

    /**
     * Smooth the lines in the chart instead of making hard corners.
     */
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    /**
     * The width of a line in the chart.
     */
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    /**
     * The opacity of the area below a line.
     */
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)

public:
    explicit LineChart(QQuickItem *parent = nullptr);

    bool smooth() const;
    void setSmooth(bool smooth);
    Q_SIGNAL void smoothChanged();

    qreal lineWidth() const;
    void setLineWidth(qreal width);
    Q_SIGNAL void lineWidthChanged();

    qreal fillOpacity() const;
    void setFillOpacity(qreal opacity);
    Q_SIGNAL void fillOpacityChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void onDataChanged() override;

private:
    void updateLineNode(LineChartNode *node, const QColor &lineColor, ChartDataSource *valueSource);

    bool m_smooth = false;
    qreal m_lineWidth = 1.0;
    qreal m_fillOpacity = 0.0;
    bool m_rangeInvalid = true;
    QVector<QVector2D> m_previousValues;
};

#endif // LINECHART_H
