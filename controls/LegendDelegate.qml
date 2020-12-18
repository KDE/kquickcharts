/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

/**
 * A delegate that can be used as part of a Legend.
 */
Control {
    id: control

    property string name
    property string shortName
    property color color
    property string value

    property real maximumValueWidth

    property Component indicator: Rectangle {
        implicitWidth: Theme.smallSpacing
        color: control.color
    }

    readonly property real minimumWidth: contentItem.minimumWidth
    readonly property real preferredWidth: contentItem.preferredWidth

    property real valueWidth
    onValueWidthChanged: Logging.deprecated("LegendDelegate", "valueWidth", "5.80", "Use maximumValueWidth instead")
    property real colorWidth
    onColorWidthChanged: Logging.deprecated("LegendDelegate", "colorWidth", "5.80", "Customize the indicator instead")
    property color valueColor
    onValueColorChanged: Logging.deprecated("LegendDelegate", "valueColor", "5.80", "Customize the delegate instead")
    property bool colorVisible
    onColorVisibleChanged: Logging.deprecated("LegendDelegate", "colorVisible", "5.80", "Use an empty indicator instead")
    property bool valueVisible
    onValueVisibleChanged: Logging.deprecated("LegendDelegate", "valueVisible", "5.80", "Customize the delegate instead")
    property real layoutWidth
    onLayoutWidthChanged: Logging.deprecated("LegendDelegate", "layoutWidth", "5.80", "Unused")

    implicitWidth: Math.max(implicitContentWidth, implicitBackgroundWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(implicitContentHeight, implicitBackgroundHeight) + topPadding + bottomPadding

    hoverEnabled: true

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    spacing: Theme.smallSpacing

    contentItem: RowLayout {
        property real actualValueWidth: control.maximumValueWidth > 0 ? control.maximumValueWidth : value.contentWidth

        property real minimumWidth: indicator.width + metrics.advanceWidth(control.shortName.slice(0,3)) + actualValueWidth +  control.spacing * 2
        property real preferredWidth: indicator.width + metrics.advanceWidth(control.name) + actualValueWidth + control.spacing * 3

        spacing: control.spacing

        Loader {
            id: indicator

            Layout.preferredWidth: item ? item.implicitWidth : 0
            Layout.fillHeight: true

            sourceComponent: control.indicator
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            implicitWidth: metrics.advanceWidth(control.name)

            Label {
                id: name
                anchors.fill: parent
                text: control.name + (control.shortName.length > 0 ? "\x9C" + control.shortName : "")
                elide: Text.ElideRight
                font: control.font
                verticalAlignment: Qt.AlignVCenter
            }

            FontMetrics {
                id: metrics
                font: control.font
            }
        }

        Label {
            id: value

            Layout.fillHeight: true
            Layout.fillWidth: true

            Layout.minimumWidth: Math.min(parent.actualValueWidth, control.width - control.spacing - indicator.width)

            text: control.value;
            elide: Text.ElideRight
            font: name.font

            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignRight
        }
    }

    ToolTip.visible: control.hovered && (name.truncated || value.truncated)
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    ToolTip.text: "%1: %2".arg(control.name).arg(control.value)
}

