/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kquickcontrols

import org.kde.quickcharts as Charts
import org.kde.quickcharts.controls as ChartsControls

Kirigami.Page {
    title: "Bar Chart"

    ListModel {
        id: barModel;
        dynamicRoles: true;

        Component.onCompleted: {
            append({label: "Item 1", value1: 0, value2: 15, value3: 20})
            append({label: "Item 2", value1: 10, value2: 25, value3: 25})
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

            ChartsControls.GridLines {
                anchors.fill: barChart

                chart: barChart

                major.visible: false

                minor.count: 4
                minor.lineWidth: 1
                minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
            }

            ChartsControls.GridLines {
                anchors.fill: barChart

                chart: barChart

                direction: ChartsControls.GridLines.Vertical;

                major.count: 1
                major.lineWidth: 2
                major.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)

                minor.count: 3
                minor.lineWidth: 1
                minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
            }

            ChartsControls.AxisLabels {
                id: yAxisLabels

                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: xAxisLabels.top
                }

                direction: ChartsControls.AxisLabels.VerticalBottomTop
                delegate: Label { text: ChartsControls.AxisLabels.label }
                source: Charts.ChartAxisSource { chart: barChart; axis: Charts.ChartAxisSource.YAxis; itemCount: 5 }
            }

            ChartsControls.AxisLabels {
                id: xAxisLabels

                anchors {
                    left: yAxisLabels.right
                    right: parent.right
                    bottom: legend.top
                }

                delegate: Label { text: ChartsControls.AxisLabels.label }
                source: Charts.ModelSource { model: barModel; roleName: "label" }
            }

            ChartsControls.Legend {
                id: legend

                anchors {
                    left: yAxisLabels.right
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: Kirigami.Units.smallSpacing
                }

                chart: barChart
            }

            Charts.BarChart {
                id: barChart
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
                    to: 10
                    automatic: true
                }

                valueSources: [
                    Charts.ModelSource { roleName: "value1"; model: barModel },
                    Charts.ModelSource { roleName: "value2"; model: barModel },
                    Charts.ModelSource { roleName: "value3"; model: barModel }
                ]

                barWidth: 10
                spacing: 2

                colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
                nameSource: Charts.ArraySource { array: ["Example 1", "Example 2", "Example 3"] }

                backgroundColor: Qt.rgba(0.0, 0.0, 0.0, 0.1)

            }
        }

        RangeEditor { label: "X Axis"; range: barChart.xRange }
        RangeEditor { label: "Y Axis"; range: barChart.yRange }

        RowLayout {
            Button { text: "Add Item"; onClicked: barModel.append({label: "New", value1: 0, value2: 0, value3: 0}) }
            Button { text: "Remove Item"; onClicked: barModel.remove(barModel.count - 1)}
            Label { text: "Bar Width" }
            SpinBox { from: -1; to: 1000; value: barChart.barWidth; onValueModified: barChart.barWidth = value; }
            Label { text: "Bar Spacing" }
            SpinBox { from: 0; to: 100; value: barChart.spacing; onValueModified: barChart.spacing = value; }
            CheckBox { text: "Stacked"; checked: barChart.stacked; onToggled: barChart.stacked = checked }
            Label { text: "Radius" }
            SpinBox { from: 0; to: 1000; value: barChart.radius; onValueModified: barChart.radius = value; }

            ComboBox {
                model: [
                    { text: "Vertical", value: Charts.BarChart.VerticalOrientation },
                    { text: "Horizontal", value: Charts.BarChart.HorizontalOrientation }
                ]

                textRole: "text"
                valueRole: "value"

                onActivated: barChart.orientation = currentValue
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            leftPadding: 1
            rightPadding: 1
            topPadding: 1
            bottomPadding: 1

            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View

            ScrollView {
                anchors.fill: parent
                ListView {
                    model: barModel;
                    delegate: ItemDelegate {
                        width: ListView.view.width
                        height: Kirigami.Units.gridUnit * 2 + Kirigami.Units.smallSpacing
                        contentItem: RowLayout {
                            Label { text: "Label" }
                            TextField {
                                Layout.preferredWidth: 75
                                text: model.label;
                                onEditingFinished: barModel.setProperty(index, "label", text)
                            }
                            Label { text: "Value 1" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: -10000; to: 10000;
                                stepSize: 1;
                                value: model.value1;
                                onValueModified: barModel.setProperty(index, "value1", value)
                            }
                            Label { text: "Value 2" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: -10000; to: 10000;
                                stepSize: 1;
                                value: model.value2;
                                onValueModified: barModel.setProperty(index, "value2", value)
                            }
                            Label { text: "Value 3" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: -10000; to: 10000;
                                stepSize: 1;
                                value: model.value3;
                                onValueModified: barModel.setProperty(index, "value3", value)
                            }
                        }
                    }
                }
            }
        }
    }
}

