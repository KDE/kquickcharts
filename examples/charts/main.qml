/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick
import QtQuick.Controls

import org.kde.kirigami as Kirigami

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
                ListElement { label: "Bar Chart"; file: "qrc:/BarChart.qml"; identifier: "bar" }
                ListElement { label: "History Proxy Source"; file: "qrc:/HistoryProxySource.qml"; identifier: "history" }
                ListElement { label: "Legend"; file: "qrc:/Legend.qml"; identifier: "legend" }
            }

            delegate: ItemDelegate {
                width: ListView.view.width

                text: model.label
                onClicked: applicationWindow().pageStack.push(model.file);

                Kirigami.Separator { width: parent.width }
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
