import QtQuick 2.9
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

RowLayout {
    id: delegate

    Layout.fillHeight: false
    Layout.preferredWidth: 0

    implicitHeight: Theme.gridUnit
    implicitWidth: Theme.gridUnit * 5

    property string name
    property color color
    property string value

    property real colorWidth: Theme.smallSpacing
    property real valueWidth: -1

    property bool colorVisible: true
    property bool valueVisible: true

    property alias font: name.font

    opacity: parent.height >= y + height // This will hide the item if it is out of bounds

    Rectangle {
        id: color

        Layout.fillHeight: true
        Layout.preferredWidth: delegate.colorWidth

        color: delegate.color
        opacity: ((parent.width - value.width - Theme.smallSpacing * 2 > 0) || parent.width < value.width) && delegate.colorVisible
    }

    Label {
        id: name

        Layout.fillWidth: true
        Layout.fillHeight: true

        text: delegate.name
        opacity: contentWidth < width
        verticalAlignment: Qt.AlignVCenter

        background: Rectangle { color: "blue"; opacity: 0.3 }
    }

    Label {
        id: value

        Layout.fillHeight: true
        Layout.preferredWidth: delegate.valueWidth

        text: delegate.value;
        font: name.font
        color: delegate.color !== undefined ? delegate.color : name.color
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignRight

        background: Rectangle { color: "red"; opacity: 0.3 }

        opacity: (parent.width >= width) && delegate.valueVisible
    }
}
