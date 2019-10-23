/*
 * This file is part of Quick Charts
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

        Layout.fillWidth: true
        Layout.fillHeight: true

        text: delegate.name
        visible: {
            if (delegate.layoutWidth < 0) {
                return true;
            }

            return delegate.layoutWidth - delegate.colorWidth - delegate.spacing * 2 >= contentWidth
        }
        verticalAlignment: Qt.AlignVCenter
    }

    Label {
        id: value

        Layout.fillHeight: true
        Layout.preferredWidth: delegate.valueWidth

        text: delegate.value;
        font: name.font
        color: delegate.valueColor
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignRight

        visible: {
            if (!delegate.valueVisible) {
                return false;
            }

            if (delegate.layoutWidth < 0) {
                return true;
            }

            return delegate.layoutWidth - delegate.colorWidth - delegate.spacing * 4 - name.contentWidth >= delegate.valueWidth
        }
    }

    Component { id: defaultIndicator; Rectangle { color: delegateColor } }
}
