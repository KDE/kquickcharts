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

Kirigami.Page {
    title: "Pie Chart"

    ListModel {
        id: pieModel;
        dynamicRoles: true;

        Component.onCompleted: {
            append({ data: 50, data2: 30, color: "red" })
            append({ data: 50, data2: 60, color: "green" })
            append({ data: 50, data2: 60, color: "blue" })
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

                valueSources: Charts.ModelSource { roleName: "data"; model: pieModel }
                colorSource: Charts.ModelSource { roleName: "color"; model: pieModel }

                thickness: 20;
                filled: false;
                backgroundColor: "gray"
            }
        }

        RangeEditor { label: "Range"; range: chart.range }

        RowLayout {
            Button { text: "Add Item"; onClicked: pieModel.append({data: 50, color: Qt.rgba(1.0, 1.0, 1.0)}) }
            Button { text: "Remove Item"; onClicked: pieModel.remove(pieModel.count - 1)}
            Label { text: "Thickness" }
            SpinBox { from: 0; to: chart.width / 2; value: chart.thickness; onValueModified: chart.thickness = value; }
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

            leftPadding: 1
            rightPadding: 1
            topPadding: 1
            bottomPadding: 1

            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View

            ScrollView {
                anchors.fill: parent
                ListView {
                    model: pieModel;
                    delegate: ItemDelegate {
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
