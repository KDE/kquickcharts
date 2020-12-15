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
     */
    property Component delegate: LegendDelegate {
        name: Charts.Legend.name
        shortName: Charts.Legend.shortName
        color: Charts.Legend.color
        value: Charts.Legend.value

        Charts.Legend.minimumWidth: implicitWidth
        Charts.Legend.maximumWidth: Theme.gridUnit * 10

        Component.onCompleted: print(Charts.Legend.index, width, height)
    }

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    implicitWidth: Math.max(contentItem.implicitWidth, background.implicitWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(contentItem.implicitHeight, background.implicitHeight) + topPadding + bottomPadding

    contentItem: Flickable {
        contentHeight: legend.implicitHeight
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        implicitHeight: legend.implicitHeight

        Charts.Legend {
            id: legend

            width: parent.width

            chart: control.chart
            delegate: control.delegate

            horizontalSpacing: Theme.smallSpacing
            verticalSpacing: Theme.largeSpacing
        }

        children: [
            ToolSeparator { width: parent.width; height: 1; visible: parent.contentY > 0 },
            ToolSeparator {
                y: parent.height - height
                width: parent.width;
                height: 1;
                visible: parent.contentY + parent.height < legend.height
            }
        ]
    }

    background: Item { }

    //implicitHeight: legend.implicitHeight

    //property alias model: legendRepeater.model
    //property alias delegate: legendRepeater.delegate
    //property alias flow: legend.flow
    //property int sourceIndex: -1

    //property var formatValue: function(input, index) { return input }

    //property bool valueVisible: false
    //property real valueWidth: -1
    //property bool colorVisible: true
    //property real colorWidth: Theme.smallSpacing

    //property string nameRole: "name"
    //property string shortNameRole: "shortName"
    //property string colorRole: "color"
    //property string valueRole: "value"

    //property Component indicator: null

    //default property alias extraItems: legend.children

    //leftPadding: 0
    //rightPadding: 0
    //topPadding: 0
    //bottomPadding: 0

    //implicitWidth: legend.implicitWidth
    //implicitHeight: legend.implicitHeight

    //contentItem: GridLayout {
        //id: legend

        //columns: flow == GridLayout.TopToBottom ? 1 : -1
        //rows: flow == GridLayout.TopToBottom ? -1 : 1

        //rowSpacing: control.spacing
        //columnSpacing: rowSpacing

        //Repeater {
            //id: legendRepeater
            //model: Charts.LegendModel { id: legendModel; chart: control.chart; sourceIndex: control.sourceIndex }

            //delegate: LegendDelegate {
                //Layout.preferredWidth: implicitWidth

                //property var itemData: typeof modelData !== "undefined" ? modelData : model
                //name: itemData[control.nameRole] !== undefined ? itemData[control.nameRole] : ""
                //shortName: itemData[control.shortNameRole] !== undefined ? itemData[control.shortNameRole] : ""
                //color: itemData[control.colorRole] !== undefined ? itemData[control.colorRole] : "white"
                //value: {
                    //if (itemData[control.valueRole] !== undefined) {
                        //if (control.formatValue.length == 2) {
                            //return control.formatValue(itemData[control.valueRole], index)
                        //} else {
                            //return control.formatValue(itemData[control.valueRole])
                        //}
                    //}
                    //return ""
                //}

                //colorVisible: control.colorVisible
                //colorWidth: control.colorWidth
                //valueVisible: control.valueVisible
                //valueWidth: control.valueWidth

                //indicator: control.indicator

                //font: control.font

                //layoutWidth: legend.flow == GridLayout.TopToBottom ? control.Layout.maximumWidth
                                //: control.Layout.maximumWidth / legendRepeater.count - control.spacing
            //}
        //}
    //}
}
