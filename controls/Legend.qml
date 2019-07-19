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

    implicitWidth: flow == GridLayout.TopToBottom ? Theme.gridUnit * 5 : legend.width
    implicitHeight: flow == GridLayout.TopToBottom ? legend.height : Theme.gridUnit

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

                Rectangle {
                    id: color
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }
                    width: control.colorWidth;
                    color: model[control.colorRole];
                    visible: ((parent.width - control.valueWidth - Theme.smallSpacing * 2 > 0) || parent.width < control.valueWidth) && control.colorVisible
                }

                Label {
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: color.right
                        right: value.left
                        leftMargin: Theme.smallSpacing
                        rightMargin: Theme.smallSpacing
                    }

                    text: model[control.nameRole];
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

                    text: control.formatValue(model[control.valueRole]);
                    font: control.font
                    color: model[control.colorRole]
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight

                    visible: (parent.width >= control.valueWidth) && control.valueVisible
                }
            }
        }
    }
}
