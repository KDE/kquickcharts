/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include "XYChart.h"

class LineChartNode;

/**
 * A line chart.
 *
 * ## Usage example
 *
 * \snippet snippets/linechart.qml example
 *
 * \image html linechart.png "The resulting bar chart."
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
     *
     * The default is 0.0. Note that if fillColorSource is set, this value is
     * ignored.
     */
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)
    /**
     * A data source that supplies color values for the line charts' fill area.
     *
     * If this is not set (the default), the normal color source will be used,
     * with the fillOpacity used as its opacity.
     */
    Q_PROPERTY(ChartDataSource *fillColorSource READ fillColorSource WRITE setFillColorSource NOTIFY fillColorSourceChanged)

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

    ChartDataSource *fillColorSource() const;
    void setFillColorSource(ChartDataSource *newFillColorSource);
    Q_SIGNAL void fillColorSourceChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void onDataChanged() override;

private:
    void updateLineNode(LineChartNode *node, const QColor &lineColor, const QColor &fillColor, ChartDataSource *valueSource);

    bool m_smooth = false;
    qreal m_lineWidth = 1.0;
    qreal m_fillOpacity = 0.0;
    bool m_rangeInvalid = true;
    QVector<QVector2D> m_previousValues;
    ChartDataSource *m_fillColorSource = nullptr;
};

#endif // LINECHART_H
