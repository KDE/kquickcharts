/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13

import org.kde.quickcharts 1.0 as Charts
import org.kde.quickcharts.controls 1.0

/**
 * A pre-made legend control that displays a legend for charts.
 */
Control {
    id: control

    /**
     * The chart to display the legend for.
     */
    property Charts.Chart chart
    /**
     * The delegate to use to display legend information.
     *
     * \sa Legend::delegate
     */
    property alias delegate: legendRepeater.delegate
    /**
     *
     */
    property alias model: legendRepeater.model

    property alias horizontalSpacing: legend.horizontalSpacing
    property alias verticalSpacing: legend.verticalSpacing

    property real maximumDelegateWidth: Theme.gridUnit * 10

    property var formatValue: function(input, index) { return input }
    property var maximumValueWidth: function(input, index) { return -1 }

    property alias preferredWidth: legend.preferredWidth

    property string nameRole: "name"
    property string shortNameRole: "shortName"
    property string colorRole: "color"
    property string valueRole: "value"

    property int flow
    onFlowChanged: Logging.deprecated("Legend", "flow", "5.82", "Legend uses a dynamic column layout now")
    property int sourceIndex
    onSourceIndexChanged: Logging.deprecated("Legend", "sourceIndex", "5.82", "Use Chart's indexingMode property")

    property bool valueVisible: false
    onValueVisibleChanged: Logging.deprecated("Legend", "valueVisible", "5.82", "Customise the delegate instead")
    property real valueWidth: -1
    onValueWidthChanged: Logging.deprecated("Legend", "valueWidth", "5.82", "Customise the delegate instead")
    property bool colorVisible: true
    onColorVisibleChanged: Logging.deprecated("Legend", "colorVisible", "5.82", "Customise the delegate instead")
    property real colorWidth: 0
    onColorWidthChanged: Logging.deprecated("Legend", "colorWidth", "5.82", "Customise the delegate instead")

    default property alias _children: legend.children

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    implicitWidth: Math.max(implicitContentWidth, implicitBackgroundWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(implicitContentHeight, implicitBackgroundHeight) + topPadding + bottomPadding

    contentItem: Flickable {
        anchors.fill: parent

        contentHeight: legend.implicitHeight
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        implicitHeight: legend.implicitHeight
        implicitWidth: legend.implicitWidth

        // Limit maximum flick velocity to ensure we can scroll one line per
        // mouse wheel "tick" when the legend's height is very constrained.
        maximumFlickVelocity: Theme.gridUnit * 50
        Charts.LegendLayout {
            id: legend

            width: parent.width

            Repeater {
                id: legendRepeater

                model: Charts.LegendModel { chart: control.chart }

                delegate: LegendDelegate {
                    property var itemData: typeof modelData !== "undefined" ? modelData : model

                    name: itemData[control.nameRole] !== undefined ? itemData[control.nameRole] : ""
                    shortName: itemData[control.shortNameRole] !== undefined ? itemData[control.shortNameRole] : ""
                    color: itemData[control.colorRole] !== undefined ? itemData[control.colorRole] : "white"
                    value: itemData[control.valueRole] !== undefined ? control.formatValue(itemData[control.valueRole], index) : ""

                    maximumValueWidth: {
                        var result = control.maximumValueWidth(model.value, index)
                        if (result > 0) {
                            return result
                        }

                        // Backward compatibility: While valueWidth is deprecated, it may still end up
                        // being set as a size hint, so we should use that rather than nothing if it is
                        // set.
                        if (control.valueWidth > 0) {
                            return control.valueWidth
                        }

                        return -1
                    }

                    Charts.LegendLayout.minimumWidth: minimumWidth
                    Charts.LegendLayout.preferredWidth: preferredWidth
                    Charts.LegendLayout.maximumWidth: Math.max(control.maximumDelegateWidth, preferredWidth)
                }
            }

            horizontalSpacing: Theme.largeSpacing
            verticalSpacing: Theme.smallSpacing
        }

        children: [
            Item {
                width: parent.width;
                height: 1;
                visible: parent.contentY > 0

                ToolSeparator {
                    anchors.left: parent.horizontalCenter
                    anchors.top: parent.bottom
                    width: 1
                    height: Theme.smallSpacing
                    transformOrigin: Item.Top
                    rotation: 45
                }

                ToolSeparator {
                    anchors.right: parent.horizontalCenter
                    anchors.top: parent.bottom
                    width: 1
                    height: Theme.smallSpacing
                    transformOrigin: Item.Top
                    rotation: -45
                }
            },
            Item {
                y: parent.height - height
                width: parent.width;
                height: 1;
                visible: parent.contentY + parent.height < legend.height

                ToolSeparator {
                    anchors.left: parent.horizontalCenter
                    anchors.bottom: parent.top
                    width: 1
                    height: Theme.smallSpacing
                    transformOrigin: Item.Bottom
                    rotation: 45
                }

                ToolSeparator {
                    anchors.right: parent.horizontalCenter
                    anchors.bottom: parent.top
                    width: 1
                    height: Theme.smallSpacing
                    transformOrigin: Item.Bottom
                    rotation: -45
                }
            }
        ]
    }
}
