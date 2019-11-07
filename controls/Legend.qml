/*
 * This file is part of KQuickCharts
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
 * A pre-made legend control that displays a legend for charts.
 */
Control {
    id: control

    /**
     * The chart to display the legend for.
     */
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

    property Component indicator: null

    default property alias extraItems: legend.children

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    implicitWidth: legend.implicitWidth
    implicitHeight: legend.implicitHeight

    contentItem: GridLayout {
        id: legend

        columns: flow == GridLayout.TopToBottom ? 1 : -1
        rows: flow == GridLayout.TopToBottom ? -1 : 1

        rowSpacing: control.spacing
        columnSpacing: rowSpacing

        Repeater {
            id: legendRepeater
            model: Charts.LegendModel { id: legendModel; chart: control.chart; sourceIndex: control.sourceIndex }

            delegate: LegendDelegate {
                Layout.preferredWidth: implicitWidth

                property var itemData: typeof modelData !== "undefined" ? modelData : model
                name: itemData[control.nameRole] !== undefined ? itemData[control.nameRole] : ""
                color: itemData[control.colorRole] !== undefined ? itemData[control.colorRole] : "white"
                value: itemData[control.valueRole] !== undefined ? control.formatValue(itemData[control.valueRole]) : ""

                colorVisible: control.colorVisible
                colorWidth: control.colorWidth
                valueVisible: control.valueVisible
                valueWidth: control.valueWidth

                indicator: control.indicator

                font: control.font

                layoutWidth: control.Layout.maximumWidth / legendRepeater.count
            }
        }
    }
}
