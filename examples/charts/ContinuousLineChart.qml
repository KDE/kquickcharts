/*
 * This file is part of KQuickCharts
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts

Kirigami.Page {
    title: "Line Chart"

    ListModel {
        id: lineModel;
        dynamicRoles: true;
    }

    Timer {
        id: updateTimer
        running: true
        repeat: true
        interval: 16
        property real lastValue: 0.0
        onTriggered: {
            var value = Math.max(0.0, Math.min(1.0, lastValue + (-0.05 + Math.random() / 10)))
            lastValue = value;
            lineModel.insert(0, {"value": value})

            var distance = chart.xRange.to - chart.xRange.from
            if(!chart.xRange.automatic && lineModel.count > distance) {
                var toRemove = lineModel.count - distance;
                lineModel.remove(lineModel.count - toRemove, toRemove)
            }
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

            Charts.LineChart {
                id: chart
                anchors.fill: parent

                xRange {
                    from: 0
                    to: 500
                    automatic: false
                }

                yRange {
                    from: 0
                    to: 1
                    automatic: false
                }

                valueSources: Charts.ModelSource { roleName: "value"; model: lineModel }

                colorSource: Charts.SingleValueSource { value: "darkRed" }

                lineWidth: 2
                fillOpacity: 0.2
            }
        }

        RowLayout {
            RangeEditor { label: "X Axis"; range: chart.xRange }
            Item { Layout.fillWidth: true }
            Button { icon.name: "media-playback-start"; enabled: !updateTimer.running; onClicked: updateTimer.start() }
            Button { icon.name: "media-playback-stop"; enabled: updateTimer.running; onClicked: updateTimer.stop() }
            Item { Layout.fillWidth: true }
            RangeEditor { label: "Y Axis"; range: chart.yRange }
        }
    }
}
