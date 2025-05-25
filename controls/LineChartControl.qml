/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick
import QtQuick.Controls

import org.kde.quickcharts as Charts
import org.kde.quickcharts.controls

/*!
  \qmltype LineChartControl
  \inqmlmodule org.kde.quickcharts.controls
  \brief A line chart with legend, grid and axis labels.
 */
Control {

    /*!
      \qmlproperty list<ChartDataSource> LineChartControl::valueSources
     */
    property alias valueSources: lineChart.valueSources

    /*!
      \qmlproperty list<variant> LineChartControl::names
     */
    property alias names: nameSource.array

    /*!
      \qmlproperty color LineChartControl::color
     */
    property alias color: colorSource.baseColor

    /*!
      \qmlproperty real LineChartControl::lineWidth
     */
    property alias lineWidth: lineChart.lineWidth

    /*!
      \qmlproperty real LineChartControl::fillOpacity
     */
    property alias fillOpacity: lineChart.fillOpacity

    /*!
      \qmlproperty bool LineChartControl::stacked
     */
    property alias stacked: lineChart.stacked

    /*!
      \qmlproperty LineChart LineChartControl::chart
     */
    property alias chart: lineChart

    /*!
      \qmlproperty Legend LineChartControl::legend
     */
    property alias legend: legend

    /*!
      \qmlproperty AxisLabels LineChartControl::xLabels
     */
    property alias xLabels: xAxisLabels

    /*!
      \qmlproperty AxisLabels LineChartControl::yLabels
     */
    property alias yLabels: yAxisLabels

    /*!
      \qmlproperty bool LineChartControl::verticalLinesVisible
     */
    property alias verticalLinesVisible: verticalLines.visible

    /*!
      \qmlproperty bool LineChartControl::horizontalLinesVisible
     */
    property alias horizontalLinesVisible: horizontalLines.visible

    /*!
      \qmlproperty real LineChartControl::xRange.from
      \qmlproperty real LineChartControl::xRange.to
      \qmlproperty bool LineChartControl::xRange.automatic
      \qmlproperty real LineChartControl::xRange.distance
      \qmlproperty real LineChartControl::xRange.minimum
      \qmlproperty real LineChartControl::xRange.increment

      \brief The range of values on the X axis.

      from: The start of this range. The default is 0.

      to: The end of this range. The default is 100.

      automatic: Whether to determine the range based on values of a chart. If true (the default), from and to are ignored and instead calculated from the minimum and maximum values of a chart's valueSources.

      distance: The distance between from and to.

      minimum: The minimum size of the range. This is mostly relevant when automatic is true. Setting this value will ensure that the range will never be smaller than this value. The default is std::numeric_limits<qreal>::min, which means minimum is disabled.

      increment: The amount with which the range increases. The total range will be limited to a multiple of this value. This is mostly useful when automatic is true. The default is 0.0, which means do not limit the range increment.
     */
    property alias xRange: lineChart.xRange

    /*!
      \qmlproperty real LineChartControl::yRange.from
      \qmlproperty real LineChartControl::yRange.to
      \qmlproperty bool LineChartControl::yRange.automatic
      \qmlproperty real LineChartControl::yRange.distance
      \qmlproperty real LineChartControl::yRange.minimum
      \qmlproperty real LineChartControl::yRange.increment

      \brief The range of values on the Y axis.

      from: The start of this range. The default is 0.

      to: The end of this range. The default is 100.

      automatic: Whether to determine the range based on values of a chart. If true (the default), from and to are ignored and instead calculated from the minimum and maximum values of a chart's valueSources.

      distance: The distance between from and to.

      minimum: The minimum size of the range. This is mostly relevant when automatic is true. Setting this value will ensure that the range will never be smaller than this value. The default is std::numeric_limits<qreal>::min, which means minimum is disabled.

      increment: The amount with which the range increases. The total range will be limited to a multiple of this value. This is mostly useful when automatic is true. The default is 0.0, which means do not limit the range increment.
     */
    property alias yRange: lineChart.yRange

    /*!
      \qmlproperty ChartAxisSource LineChartControl::xAxisSource
     */
    property alias xAxisSource: xAxisLabels.source

    /*!
      \qmlproperty ChartAxisSource LineChartControl::yAxisSource
     */
    property alias yAxisSource: yAxisLabels.source

    /*!
      \qmlproperty Component LineChartControl::pointDelegate
     */
    property alias pointDelegate: lineChart.pointDelegate

    /*!
      \qmlproperty bool LineChartControl::highlightEnabled
     */
    property alias highlightEnabled: legend.highlightEnabled

    background: Rectangle { color: Theme.backgroundColor }

    contentItem: Item {
        anchors.fill: parent;

        GridLines {
            id: horizontalLines

            anchors.fill: lineChart

            chart: lineChart

            major.frequency: 2
            major.lineWidth: 2
            major.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)

            minor.frequency: 1
            minor.lineWidth: 1
            minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
        }

        GridLines {
            id: verticalLines

            anchors.fill: lineChart

            chart: lineChart

            direction: GridLines.Vertical;

            major.count: 1
            major.lineWidth: 2
            major.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)

            minor.count: 3
            minor.lineWidth: 1
            minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
        }

        AxisLabels {
            id: yAxisLabels

            anchors {
                left: parent.left
                top: parent.top
                bottom: xAxisLabels.top
            }

            direction: AxisLabels.VerticalBottomTop
            delegate: Label { text: AxisLabels.label }
            source: Charts.ChartAxisSource { chart: lineChart; axis: Charts.ChartAxisSource.YAxis; itemCount: 5 }
        }

        AxisLabels {
            id: xAxisLabels

            anchors {
                left: yAxisLabels.visible ? yAxisLabels.right : parent.left
                right: parent.right
                bottom: legend.top
            }

            delegate: Label { text: AxisLabels.label }
            source: Charts.ChartAxisSource { chart: lineChart; axis: Charts.ChartAxisSource.XAxis; itemCount: 5 }
        }

        Legend {
            id: legend

            anchors {
                left: yAxisLabels.visible ? yAxisLabels.right : parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: Theme.smallSpacing
            }

            chart: lineChart
        }

        Charts.LineChart {
            id: lineChart
            anchors {
                top: parent.top
                left: yAxisLabels.visible ? yAxisLabels.right : parent.left
                right: parent.right
                bottom: xAxisLabels.visible ? xAxisLabels.top : legend.top
            }

            xRange.automatic: true
            yRange.automatic: true

            colorSource: Charts.ColorGradientSource { id: colorSource; baseColor: Theme.highlightColor; itemCount: lineChart.valueSources.length }
            nameSource: Charts.ArraySource { id: nameSource; array: ["1", "2", "3", "4", "5"] }

            highlight: legend.highlightedIndex
        }
    }
}
