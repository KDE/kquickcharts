import QtQuick 2.9
import QtQuick.Controls 2.2

import org.kde.kirigami 2.2 as Kirigami

Kirigami.ApplicationWindow {
    title: "Charts Example"

    pageStack.initialPage: Kirigami.ScrollablePage {
        title: "Charts"

        ListView {

            model: ListModel {
                ListElement { label: "Pie Chart"; file: "qrc:/PieChart.qml" }
                ListElement { label: "Line Chart"; file: "qrc:/LineChart.qml" }
            }

            delegate: Kirigami.BasicListItem {
                label: model.label
                onClicked: applicationWindow().pageStack.push(model.file);
            }
        }
    }
}
