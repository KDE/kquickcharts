import QtQuick 2.9
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

Control {
    id: control

    property alias chart: legendModel.chart
    property alias model: legendRepeater.model
    property alias delegate: legendRepeater.delegate
    property alias flow: legend.flow

    property real valueWidth: Theme.gridUnit * 2
    property var formatValue: function(input) { return input }

    property bool valueVisible: false
    property bool colorVisible: true
    property real colorWidth: Theme.smallSpacing

    property string nameRole: "name"
    property string colorRole: "color"
    property string valueRole: "value"

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    implicitWidth: flow == GridLayout.TopToBottom ? Theme.gridUnit * 5 : legendRepeater.count * Theme.gridUnit * 5
    implicitHeight: flow == GridLayout.TopToBottom ? legendRepeater.count * Theme.gridUnit : Theme.gridUnit

    contentItem: GridLayout {
        id: legend

        columns: flow == GridLayout.TopToBottom ? 1 : -1
        rows: flow == GridLayout.TopToBottom ? -1 : 1

        rowSpacing: 0
        columnSpacing: 0

        Repeater {
            id: legendRepeater
            model: Charts.LegendModel { id: legendModel; chart: chart }

            Item {
                id: delegate

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 0

                implicitHeight: Theme.gridUnit
                implicitWidth: Theme.gridUnit * 5

                property var itemData: modelData !== undefined ? modelData : model
                property var name: itemData[control.nameRole] !== undefined ? itemData[control.nameRole] : ""
                property var color: itemData[control.colorRole] !== undefined ? itemData[control.colorRole] : "white"
                property var value: itemData[control.valueRole] !== undefined ? itemData[control.valueRole] : ""

                Rectangle {
                    id: color
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }
                    width: control.colorWidth
                    color: delegate.color
                    visible: ((parent.width - control.valueWidth - Theme.smallSpacing * 2 > 0) || parent.width < control.valueWidth) && control.colorVisible
                }

                Label {
                    id: name
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: color.right
                        right: value.left
                        leftMargin: Theme.smallSpacing
                        rightMargin: Theme.smallSpacing
                    }

                    text: delegate.name
                    font: control.font
                    visible: contentWidth < width
                    verticalAlignment: Qt.AlignVCenter
                }

                Label {
                    id: value
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.right
                        rightMargin: Theme.smallSpacing
                    }
                    width: control.valueWidth

                    text: control.formatValue(delegate.value);
                    font: control.font
                    color: itemData[control.colorRole] !== undefined ? delegate.color : name.color
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight

                    visible: (parent.width >= control.valueWidth) && control.valueVisible
                }
            }
        }
    }
}
