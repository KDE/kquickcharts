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
