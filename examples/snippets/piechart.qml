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

Charts.PieChart {
    width: 400
    height: 300

    valueSources: Charts.ModelSource {
        roleName: "data";
        model: listModel
    }
    colorSource: Charts.ArraySource { array: ["red", "green", "blue", "yellow", "cyan"] }
    nameSource: Charts.ArraySource { array: ["Red", "Green", "Blue", "Yellow", "Cyan"] }

    thickness: 50

    ListModel {
        id: listModel
        ListElement { data: 12 }
        ListElement { data: 19 }
        ListElement { data: 10 }
        ListElement { data: 13 }
        ListElement { data: 13 }
    }
}
//! [example]
