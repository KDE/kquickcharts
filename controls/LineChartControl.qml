import QtQuick 2.9
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

Control {

    property alias valueSources: lineChart.valueSources
    property alias names: nameSource.array

    property alias lineWidth: lineChart.lineWidth
    property alias fillOpacity: lineChart.fillOpacity
    property alias stacked: lineChart.stacked

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
                left: yAxisLabels.right
                right: parent.right
                bottom: legend.top
            }

            delegate: Label { text: Charts.AxisLabels.label }
            source: Charts.ModelSource { model: lineModel; roleName: "label" }
        }

        Legend {
            id: legend

            anchors {
                left: yAxisLabels.right
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
                left: yAxisLabels.right
                right: parent.right
                bottom: xAxisLabels.top
            }

            xRange.automatic: true
            yRange.automatic: true

            colorSource: Charts.ColorGradientSource { baseColor: Theme.highlightColor; itemCount: lineChart.valueSources.length }
            nameSource: Charts.ArraySource { id: nameSource; array: ["1", "2", "3", "4", "5"] }
        }
    }
}
