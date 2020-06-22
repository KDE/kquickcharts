/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.9
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

/**
 * A delegate that can be used as part of a Legend.
 */
RowLayout {
    id: delegate

    Layout.fillHeight: false
    Layout.preferredWidth: 0

    property string name
    property string shortName
    property color color
    property string value
    property color valueColor: name.color

    property real colorWidth: Theme.smallSpacing
    property real valueWidth: -1

    property bool colorVisible: true
    property bool valueVisible: true

    property alias font: name.font

    property Component indicator: null

    property real layoutWidth: -1

    spacing: Theme.smallSpacing

    opacity: parent.height >= y + height // This will hide the item if it is out of bounds

    Loader {
        Layout.preferredHeight: name.contentHeight
        Layout.preferredWidth: delegate.colorWidth

        visible: delegate.colorVisible

        property color delegateColor: delegate.color

        sourceComponent: delegate.indicator != null ? delegate.indicator : defaultIndicator

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true

            ToolTip.visible: mouse.containsMouse && (!name.visible || !value.visible)
            ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
            ToolTip.text: "%1: %2".arg(delegate.name).arg(delegate.value)
        }
    }

    Label {
        id: name
        visible: delegate.width - delegate.colorWidth - delegate.spacing - value.implicitWidth > 0
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumWidth: 0

        text: delegate.name + (delegate.shortName.length > 0 ? "\x9C" + delegate.shortName : "")
        elide: Text.ElideRight

        verticalAlignment: Qt.AlignVCenter
    }

    Label {
        id: value
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.minimumWidth: name.visible ? implicitWidth : 0
        Layout.preferredWidth: implicitWidth

        text: delegate.value;
        elide: Text.ElideRight
        font: name.font
        color: delegate.valueColor
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: name.visible ? Qt.AlignRight :  Qt.AlignLeft
    }

    Component { id: defaultIndicator; Rectangle { color: delegateColor } }
}
