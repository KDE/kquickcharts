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
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts

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
