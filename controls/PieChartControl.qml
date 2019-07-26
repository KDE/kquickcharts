import QtQuick 2.9
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

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
