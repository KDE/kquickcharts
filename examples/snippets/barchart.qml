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

//! [example]
import QtQuick 2.11
import QtQuick.Controls 2.11

import org.kde.quickcharts 1.0 as Charts

Charts.BarChart {
    id: barChart

    width: 400
    height: 300

    valueSources: [
        Charts.ModelSource { roleName: "value1"; model: listModel },
        Charts.ModelSource { roleName: "value2"; model: listModel },
        Charts.ModelSource { roleName: "value3"; model: listModel }
    ]

    colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
    nameSource: Charts.ArraySource { array: ["Example 1", "Example 2", "Example 3"] }

    barWidth: 20

    ListModel {
        id: listModel

        ListElement { value1: 19; value2: 2; value3: 6 }
        ListElement { value1: 14; value2: 20; value3: 17 }
        ListElement { value1: 4; value2: 10; value3: 11 }
        ListElement { value1: 5; value2: 11; value3: 9 }
        ListElement { value1: 20; value2: 7; value3: 13 }
    }
}
//! [example]
