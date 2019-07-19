import QtQuick 2.9
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

Control {

    contentItem: Item {
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

        ChartsControls.Legend {
            id: legend

            anchors {
                left: yAxisLabels.right
                right: parent.right
                bottom: parent.bottom
                bottomMargin: Kirigami.Units.smallSpacing
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

            xRange {
                from: 0
                to: 10
                automatic: true
            }

            yRange {
                from: 0
                to: 100
                automatic: true
            }

            valueSources: [
                Charts.ModelSource { roleName: "value1"; model: lineModel },
                Charts.ModelSource { roleName: "value2"; model: lineModel },
                Charts.ModelSource { roleName: "value3"; model: lineModel }
            ]

            colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
            nameSource: Charts.ArraySource { array: ["Example 1", "Example 2", "Example 3"] }
        }
    }
}
