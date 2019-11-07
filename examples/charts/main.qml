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

import org.kde.kirigami 2.2 as Kirigami

Kirigami.ApplicationWindow {
    id: window
    title: "Charts Example"

    pageStack.initialPage: Kirigami.ScrollablePage {
        title: "Charts"

        ListView {

            model: ListModel {
                id: pagesModel
                ListElement { label: "Pie Chart"; file: "qrc:/PieChart.qml"; identifier: "pie" }
                ListElement { label: "Line Chart"; file: "qrc:/LineChart.qml"; identifier: "line" }
                ListElement { label: "Continuous Line Chart"; file: "qrc:/ContinuousLineChart.qml"; identifier: "continuous" }
                ListElement { label: "Bar Chart"; file: "qrc:/BarChart.qml"; identifier: "bar" }
            }

            delegate: Kirigami.BasicListItem {
                label: model.label
                onClicked: applicationWindow().pageStack.push(model.file);
            }
        }
    }

    Component.onCompleted: {
        if (__commandLinePage !== null) {
            for (var i = 0; i < pagesModel.count; ++i) {
                var item = pagesModel.get(i);
                if (item.identifier == __commandLinePage || item.label == __commandLinePage) {
                    window.pageStack.push(item.file);
                    return;
                }
            }
        }
    }
}
