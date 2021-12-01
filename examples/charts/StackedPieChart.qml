/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts

Kirigami.Page {
    title: "Pie Chart"

    ListModel {
        id: pieModel;
        dynamicRoles: true;

        Component.onCompleted: {
            append({ data: 0, data2: 0, data3: 0, color: "transparent" })
            append({ data: 10, data2: 10, data3: 33, color: "red" })
            append({ data: 23, data2: 23, data3: 0, color: "transparent" })
            append({ data: 5, data2: 11, data3: 33, color: "green" })
            append({ data: 28, data2: 22, data3: 0, color: "transparent" })
            append({ data: 11, data2: 22, data3: 33, color: "blue" })
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.largeSpacing

        Kirigami.AbstractCard {
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.preferredWidth: 600
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Charts.PieChart {
                id: chart
                anchors.fill: parent
                anchors.margins: Kirigami.Units.smallSpacing;

                range.to: 150

                valueSources: [
                    Charts.ModelSource { roleName: "data"; model: pieModel },
                    Charts.ModelSource { roleName: "data2"; model: pieModel },
                    Charts.ModelSource { roleName: "data3"; model: pieModel }
                ]
                colorSource: Charts.ModelSource { roleName: "color"; model: pieModel }
                nameSource: Charts.ArraySource { array: ["Data 1", "Data 2", "Data 3"] }

                thickness: 50;
                spacing: 1;

                filled: false;
                backgroundColor: "transparent"

                property var point: chart.pointAt(hover.point.position)

                HoverHandler {
                    id: hover
                }

                ToolTip.visible: point.valid
                ToolTip.text: point.name + " " + point.value
            }
        }

        RangeEditor { label: "Range"; range: chart.range }

        RowLayout {
            Button { text: "Add Item"; onClicked: pieModel.append({data: 50, color: Qt.rgba(1.0, 1.0, 1.0)}) }
            Button { text: "Remove Item"; onClicked: pieModel.remove(pieModel.count - 1)}
            Label { text: "Thickness" }
            SpinBox { from: 0; to: chart.width / 2; value: chart.thickness; onValueModified: chart.thickness = value; }
            Label { text: "Spacing" }
            SpinBox { from: 0; to: chart.width / 2; value: chart.spacing; onValueModified: chart.spacing = value; }
            CheckBox { text: "Filled"; checked: chart.filled; onCheckedChanged: chart.filled = checked }
        }

        RowLayout {
            CheckBox { text: "Smooth Ends"; checked: chart.smoothEnds; onCheckedChanged: chart.smoothEnds = checked }
            Label { text: "From Angle" }
            SpinBox { from: -360; to: 360; value: chart.fromAngle; onValueModified: chart.fromAngle = value; }
            Label { text: "To Angle" }
            SpinBox { from: -360; to: 360; value: chart.toAngle; onValueModified: chart.toAngle = value; }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                ListView {
                    model: pieModel;
                    delegate: Kirigami.BasicListItem {
                        width: ListView.view.width
                        height: Kirigami.Units.gridUnit * 2 + Kirigami.Units.smallSpacing
                        contentItem: RowLayout {
                            Label { text: "Value" }
                            SpinBox {
                                Layout.preferredWidth: parent.width * 0.15
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.data;
                                onValueModified: pieModel.setProperty(index, "data", value)
                            }
                            SpinBox {
                                Layout.preferredWidth: parent.width * 0.15
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.data2;
                                onValueModified: pieModel.setProperty(index, "data2", value)
                            }
                            SpinBox {
                                Layout.preferredWidth: parent.width * 0.15
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.data3;
                                onValueModified: pieModel.setProperty(index, "data3", value)
                            }

                            Label { text: "Color" }
                            ColorButton {
                                color: model.color;
                                showAlphaChannel: true;
                                onColorChanged: pieModel.setProperty(index, "color", color)
                            }
                        }
                    }
                }
            }
        }
    }
}
