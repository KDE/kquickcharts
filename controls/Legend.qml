import QtQuick 2.9
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

Control {
    id: control

    property Charts.Chart chart
    property alias model: legendRepeater.model
    property alias delegate: legendRepeater.delegate
    property alias flow: legend.flow
    property int sourceIndex: -1

    property var formatValue: function(input) { return input }

    property bool valueVisible: false
    property real valueWidth: -1
    property bool colorVisible: true
    property real colorWidth: Theme.smallSpacing

    property string nameRole: "name"
    property string colorRole: "color"
    property string valueRole: "value"

    default property alias extraItems: legend.children

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
            model: Charts.LegendModel { id: legendModel; chart: control.chart; sourceIndex: control.sourceIndex }

            delegate: LegendDelegate {
                property var itemData: typeof modelData !== "undefined" ? modelData : model
                name: itemData[control.nameRole] !== undefined ? itemData[control.nameRole] : ""
                color: itemData[control.colorRole] !== undefined ? itemData[control.colorRole] : "white"
                value: itemData[control.valueRole] !== undefined ? control.formatValue(itemData[control.valueRole]) : ""

                colorVisible: control.colorVisible
                colorWidth: control.colorWidth
                valueVisible: control.valueVisible
                valueWidth: control.valueWidth

                font: control.font
            }
        }
    }
}
