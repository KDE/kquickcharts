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

#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include "XYChart.h"

BEGIN_NAMESPACE

class LineChartNode;

/**
 * A line chart.
 */
class LineChart : public XYChart
{
    Q_OBJECT

    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)

public:
    LineChart(QQuickItem *parent = nullptr);
    ~LineChart();

    bool smooth() const;
    qreal lineWidth() const;
    qreal fillOpacity() const;

public Q_SLOTS:
    void setSmooth(bool smooth);
    void setLineWidth(qreal width);
    void setFillOpacity(qreal opacity);

Q_SIGNALS:
    void smoothChanged();
    void lineWidthChanged();
    void fillOpacityChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void onDataChanged() override;

private:
    void updateLineNode(LineChartNode *node, const QColor& lineColor, ChartDataSource *valueSource);

    bool m_smooth = false;
    qreal m_lineWidth = 1.0;
    qreal m_fillOpacity = 0.0;
    bool m_rangeInvalid = true;
    QVector<QVector2D> m_previousValues;
};

END_NAMESPACE

#endif // LINECHART_H
