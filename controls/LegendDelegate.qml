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
    onValueWidthChanged: Logging.deprecated("LegendDelegate", "valueWidth", "5.82", "Use maximumValueWidth instead")
    property real colorWidth
    onColorWidthChanged: Logging.deprecated("LegendDelegate", "colorWidth", "5.82", "Customize the indicator instead")
    property color valueColor
    onValueColorChanged: Logging.deprecated("LegendDelegate", "valueColor", "5.82", "Customize the delegate instead")
    property bool colorVisible: true
    onColorVisibleChanged: Logging.deprecated("LegendDelegate", "colorVisible", "5.82", "Use an empty indicator instead")
    property bool valueVisible: true
    onValueVisibleChanged: Logging.deprecated("LegendDelegate", "valueVisible", "5.82", "Customize the delegate instead")
    property real layoutWidth
    onLayoutWidthChanged: Logging.deprecated("LegendDelegate", "layoutWidth", "5.82", "Unused")

    implicitHeight: Math.max(implicitContentHeight, implicitBackgroundHeight) + topPadding + bottomPadding

    // Note: Do not use implicitContentWidth here as it indirectly depends on the item width and will lead to a
    // nasty infinite layout loop. Instead use something more stable that doesn't change depending on the item
    // width.
    implicitWidth: Math.max(contentItem.preferredWidth, implicitBackgroundWidth) + leftPadding + rightPadding

    hoverEnabled: true

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    spacing: Theme.smallSpacing

    contentItem: RowLayout {
        property real actualValueWidth: control.maximumValueWidth > 0 ? control.maximumValueWidth : value.contentWidth
        property real minimumValueWidth: control.width - indicator.width - control.spacing

        property real minimumWidth: indicator.width + actualValueWidth + control.spacing
        property real preferredWidth: indicator.width + labelContainer.implicitWidth + actualValueWidth + control.spacing * 2

        spacing: control.spacing

        Loader {
            id: indicator

            Layout.preferredWidth: item ? item.implicitWidth : 0
            Layout.fillHeight: true

            sourceComponent: control.colorVisible ? control.indicator : null
        }

        Item {
            id: labelContainer

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

            Layout.minimumWidth: Math.min(parent.actualValueWidth, parent.minimumValueWidth)

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

