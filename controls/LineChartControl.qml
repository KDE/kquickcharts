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

import QtQuick 2.9
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

Control {

    property alias valueSources: lineChart.valueSources
    property alias names: nameSource.array
    property alias color: colorSource.baseColor

    property alias lineWidth: lineChart.lineWidth
    property alias fillOpacity: lineChart.fillOpacity
    property alias stacked: lineChart.stacked

    property alias chart: lineChart
    property alias legend: legend
    property alias xLabels: xAxisLabels
    property alias yLabels: yAxisLabels

    property alias verticalLinesVisible: verticalLines.visible
    property alias horizontalLinesVisible: horizontalLines.visible

    property alias xRange: lineChart.xRange
    property alias yRange: lineChart.yRange

    property alias xAxisSource: xAxisLabels.source
    property alias yAxisSource: yAxisLabels.source

    background: Rectangle { color: Theme.backgroundColor }

    contentItem: Item {
        anchors.fill: parent;

        Charts.GridLines {
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

        Charts.GridLines {
            id: verticalLines

            anchors.fill: lineChart

            chart: lineChart

            direction: Charts.GridLines.Vertical;

            major.count: 1
            major.lineWidth: 2
            major.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)

            minor.count: 3
            minor.lineWidth: 1
            minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
        }

        Charts.AxisLabels {
            id: yAxisLabels

            anchors {
                left: parent.left
                top: parent.top
                bottom: xAxisLabels.top
            }

            direction: Charts.AxisLabels.VerticalBottomTop
            delegate: Label { text: Charts.AxisLabels.label }
            source: Charts.ChartAxisSource { chart: lineChart; axis: Charts.ChartAxisSource.YAxis; itemCount: 5 }
        }

        Charts.AxisLabels {
            id: xAxisLabels

            anchors {
                left: yAxisLabels.visible ? yAxisLabels.right : parent.left
                right: parent.right
                bottom: legend.top
            }

            delegate: Label { text: Charts.AxisLabels.label }
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
        }
    }
}
