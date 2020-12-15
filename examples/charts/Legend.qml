/*
 * SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0 as ChartsControls

Kirigami.Page {
    title: "Legend"

    ListModel {
        id: lineModel;
        dynamicRoles: true;

        Component.onCompleted: {
            append({value1: 10, value2:  9, value3:  3})
            append({value1: 10, value2:  5, value3: 17})
            append({value1: 10, value2: 16, value3:  8})
            append({value1: 10, value2: 12, value3: 11})
            append({value1: 10, value2:  8, value3: 11})
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.largeSpacing

        Kirigami.AbstractCard {
            Layout.fillHeight: false
            Layout.fillWidth: true
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            contentItem: Item {
                Charts.PieChart {
                    id: chart
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                        bottom: legend.top
                        bottomMargin: Kirigami.Units.largeSpacing
                    }

                    valueSources: [
                        Charts.ModelSource { roleName: "value1"; model: lineModel },
                        Charts.ModelSource { roleName: "value2"; model: lineModel },
                        Charts.ModelSource { roleName: "value3"; model: lineModel }
                    ]

                    colorSource: Charts.ColorGradientSource {
                        baseColor: Kirigami.Theme.highlightColor
                        itemCount: lineModel.count * 3
                    }
                    nameSource: Charts.ArraySource {
                        array: ["Item 1", "Item 2", "Item 3", "Item 4", "Item 5"]
                        wrap: true
                    }
                    shortNameSource: Charts.ArraySource {
                        array: ["I1", "I2", "I3", "I4", "I5"]
                        wrap: true
                    }

                    indexingMode: Charts.Chart.IndexAllValues
                }

                ChartsControls.Legend {
                    id: legend

                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    height: Math.min(implicitHeight, parent.height * 0.3);

                    chart: chart
                }
            }
        }
    }
}
