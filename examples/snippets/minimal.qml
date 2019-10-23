/*
 * This file is part of Quick Charts
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

import QtQuick 2.11
import QtQuick.Controls 2.11

import org.kde.quickcharts 1.0 as Charts

ApplicationWindow {
    width: 500
    height: 500

    Rectangle {
        anchors.centerIn: parent
        width: 300
        height: 200
        border.width: 2

        Charts.LineChart {
            anchors.fill: parent

            colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
            nameSource: Charts.ArraySource { array: ["First", "Second", "Third"] }

            valueSources: [
                Charts.ArraySource { array: [1, 2, 2, 1] },
                Charts.ArraySource { array: [2, 5, 2, 5] },
                Charts.ArraySource { array: [5, 4, 3, 4] }
            ]
        }
    }
}

