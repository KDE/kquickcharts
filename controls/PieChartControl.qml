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

/**
 * A pie chart with text in the middle.
 */
Control {
    property alias valueSources: pie.valueSources
    property alias names: nameSource.array
    property alias color: colorSource.baseColor
    property alias range: pie.range
    property alias chart: pie

    property alias text: centerText.text

    implicitWidth: Theme.gridUnit * 5
    implicitHeight: Theme.gridUnit * 5

    contentItem: Item {
        Charts.PieChart {
            id: pie

            anchors.fill: parent

            nameSource: Charts.ArraySource { id: nameSource; array: [ ] }
            colorSource: Charts.ColorGradientSource { id: colorSource; baseColor: Theme.highlightColor; itemCount: pie.valueSources.length }
        }

        Label {
            id: centerText

            anchors.centerIn: parent;

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
    }
}
