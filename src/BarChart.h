/*
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BARCHART_H
#define BARCHART_H

#include "XYChart.h"

struct Bar;

/**
 * An item to render a bar chart.
 *
 * This chart renders
 *
 * ## Usage example
 *
 * \snippet snippets/barchart.qml example
 *
 * \image html barchart.png "The resulting bar chart."
 */
class BarChart : public XYChart
{
    Q_OBJECT

public:
    /**
     * Helper enum to provide an easy way to set barWidth to auto.
     */
    enum WidthMode { AutoWidth = -2 };
    Q_ENUM(WidthMode)

    explicit BarChart(QQuickItem *parent = nullptr);

    /**
     * The spacing between bars for each value source.
     *
     * Note that spacing between each X axis value is determined automatically
     * based on barWidth, spacing and total chart width. The default is 0.
     */
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    qreal spacing() const;
    void setSpacing(qreal newSpacing);
    Q_SIGNAL void spacingChanged();

    /**
     * The width of individual bars in the chart.
     *
     * If set to WidthMode::AutoWidth (also the default), the width will be
     * calculated automatically based on total chart width and item count.
     */
    Q_PROPERTY(qreal barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)
    qreal barWidth() const;
    void setBarWidth(qreal newBarWidth);
    Q_SIGNAL void barWidthChanged();

protected:
    /**
     * Reimplemented from QQuickItem.
     */
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *) override;
    /**
     * Reimplemented from Chart.
     */
    void onDataChanged() override;

private:
    QVector<Bar> calculateBars();

    qreal m_spacing = 0.0;
    qreal m_barWidth = AutoWidth;
    QVector<QVector<QPair<qreal, QColor>>> m_values;
};

#endif // BARCHART_H
