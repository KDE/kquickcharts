/*
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BARCHART_H
#define BARCHART_H

#include <qqmlregistration.h>

#include "XYChart.h"

struct Bar;

/*!
 * \qmltype BarChart
 * \inherits XYChart
 * \inqmlmodule org.kde.quickcharts
 *
 * \brief An item to render a bar chart.
 *
 * This chart renders
 *
 * \section1 Usage example
 *
 * \snippet barchart.qml example
 *
 * \image barchart.png The resulting bar chart.
 */
class QUICKCHARTS_EXPORT BarChart : public XYChart
{
    Q_OBJECT
    QML_ELEMENT

public:
    /*!
     * \enum BarChart::WidthMode
     *
     * Helper enum to provide an easy way to set barWidth to auto.
     *
     * \value AutoWidth
     *        Automatically calculate the width of bars.
     */
    enum WidthMode { AutoWidth = -2 };
    Q_ENUM(WidthMode)

    /*!
     * \enum BarChart::Orientation
     *
     * Bar direction.
     *
     * \value HorizontalOrientation
     *        Bars are oriented horizontally, with low values left and high values right.
     * \value VerticalOrientation
     *        Bars are oriented vertically, with low values at the bottom and high values at the top.
     */
    enum Orientation {
        HorizontalOrientation = Qt::Horizontal,
        VerticalOrientation = Qt::Vertical
    };
    Q_ENUM(Orientation)

    explicit BarChart(QQuickItem *parent = nullptr);

    /*!
     * \qmlproperty real BarChart::spacing
     * \brief The spacing between bars for each value source.
     *
     * Note that spacing between each X axis value is determined automatically
     * based on barWidth, spacing and total chart width. The default is 0.
     */
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    qreal spacing() const;
    void setSpacing(qreal newSpacing);
    Q_SIGNAL void spacingChanged();

    /*!
     * \qmlproperty real BarChart::barWidth
     * \brief The width of individual bars in the chart.
     *
     * If set to BarChart.AutoWidth (also the default), the width will be
     * calculated automatically based on total chart width and item count.
     */
    Q_PROPERTY(qreal barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)
    qreal barWidth() const;
    void setBarWidth(qreal newBarWidth);
    Q_SIGNAL void barWidthChanged();

    /*!
     * \qmlproperty real BarChart::radius
     * \brief The radius of the ends of bars in the chart in pixels.
     *
     * By default this is 0, which means no rounding will be done.
     */
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    qreal radius() const;
    void setRadius(qreal newRadius);
    Q_SIGNAL void radiusChanged();

    /*!
     * \qmlproperty enumeration BarChart::orientation
     * \qmlenumeratorsfrom BarChart::Orientation
     * \brief The orientation of bars in the chart.
     *
     * By default this is BarChart.VerticalOrientation.
     */
    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Orientation orientation() const;
    void setOrientation(Orientation newOrientation);
    Q_SIGNAL void orientationChanged();

    /*!
     * \qmlproperty color BarChart::backgroundColor
     * \brief The background color of bars in the chart.
     *
     * By default this is Qt::transparent. If set to something non-transparent,
     * the chart will render backgrounds for the bars. These backgrounds will
     * have the same width as the bars but stretch the full height.
     */
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);
    Q_SIGNAL void backgroundColorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *) override;
    void onDataChanged() override;

private:
    QList<Bar> calculateBars();

    qreal m_spacing = 0.0;
    qreal m_barWidth = AutoWidth;
    qreal m_radius = 0.0;
    Orientation m_orientation = VerticalOrientation;
    bool m_orientationChanged = false;
    struct BarData {
        qreal value = 0;
        QColor color;
    };
    QList<QList<BarData>> m_barDataItems;
    QColor m_backgroundColor = Qt::transparent;
};

#endif // BARCHART_H
