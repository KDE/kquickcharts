import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0 as ChartsControls

Kirigami.Page {
    title: "Line Chart"

    ListModel {
        id: lineModel;
        dynamicRoles: true;

        Component.onCompleted: {
            append({label: "Item 1", value1: 10, value2: 15, value3: 20})
            append({label: "Item 2", value1: 15, value2: 25, value3: 25})
            append({label: "Item 3", value1: 15, value2: 20, value3: 30})
            append({label: "Item 4", value1: 10, value2: 10, value3: 35})
            append({label: "Item 5", value1: 20, value2: 5, value3: 40})
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.largeSpacing

        Kirigami.AbstractCard {
            Layout.fillHeight: false
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Charts.GridLines {
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

        RowLayout {
            RangeEditor { label: "X Axis"; range: lineChart.xRange }
            Item { Layout.fillWidth: true }
            RangeEditor { label: "Y Axis"; range: lineChart.yRange }
        }

        RowLayout {
            Button { text: "Add Item"; onClicked: lineModel.append({label: "New", value1: 0, value2: 0, value3: 0}) }
            Button { text: "Remove Item"; onClicked: lineModel.remove(lineModel.count - 1)}
            Label { text: "Line Width" }
            SpinBox { from: 0; to: 1000; value: lineChart.lineWidth; onValueModified: lineChart.lineWidth = value; }
            Label { text: "Fill Opacity" }
            SpinBox { from: 0; to: 100; value: lineChart.fillOpacity * 100; onValueModified: lineChart.fillOpacity = value / 100; }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                ListView {
                    model: lineModel;
                    delegate: Kirigami.BasicListItem {
                        width: ListView.view.width
                        height: Kirigami.Units.gridUnit * 2 + Kirigami.Units.smallSpacing
                        contentItem: RowLayout {
                            Label { text: "Label" }
                            TextField {
                                Layout.preferredWidth: 75
                                text: model.label;
                                onEditingFinished: lineModel.setProperty(index, "label", text)
                            }
                            Label { text: "Value 1" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: -10000; to: 10000;
                                stepSize: 1;
                                value: model.value1;
                                onValueModified: lineModel.setProperty(index, "value1", value)
                            }
                            Label { text: "Value 2" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: -10000; to: 10000;
                                stepSize: 1;
                                value: model.value2;
                                onValueModified: lineModel.setProperty(index, "value2", value)
                            }
                            Label { text: "Value 3" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: -10000; to: 10000;
                                stepSize: 1;
                                value: model.value3;
                                onValueModified: lineModel.setProperty(index, "value3", value)
                            }
                        }
                    }
                }
            }
        }
    }
}
