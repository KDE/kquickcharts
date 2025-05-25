/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick
import QtQuick.Controls

import org.kde.quickcharts as Charts
import org.kde.quickcharts.controls

/*!
  \qmltype PieChartControl
  \inqmlmodule org.kde.quickcharts.controls
  \brief A pie chart with text in the middle.
 */
Control {

    /*!
      \qmlproperty list<ChartDataSource> PieChartControl::valueSources
     */
    property alias valueSources: pie.valueSources

    /*!
      \qmlproperty list<variant> PieChartControl::names
     */
    property alias names: nameSource.array

    /*!
      \qmlproperty color PieChartControl::color
     */
    property alias color: colorSource.baseColor

    /*!
      \qmlproperty real PieChartControl::range.from
      \qmlproperty real PieChartControl::range.to
      \qmlproperty bool PieChartControl::range.automatic
      \qmlproperty real PieChartControl::range.distance
      \qmlproperty real PieChartControl::range.minimum
      \qmlproperty real PieChartControl::range.increment

      The range of values to display in this PieChartControl.

      from: The start of this range. The default is 0.

      to: The end of this range. The default is 100.

      automatic: Whether to determine the range based on values of a chart. If true (the default), from and to are ignored and instead calculated from the
      minimum and maximum values of a chart's valueSources.

      distance: The distance between from and to

      minimum: The minimum size of the range. This is mostly relevant when automatic is true. Setting this value will
      ensure that the range will never be smaller than this value. The default
      is std::numeric_limits<qreal>::min, which means minimum is disabled.

      increment: The amount with which the range increases. The total range will be limited to a multiple of this value.  This is mostly useful when automatic
      is true. The default is 0.0, which means do not limit the range increment.

      When set to "automatic", the values will be divided across the entire
      chart.
     */
    property alias range: pie.range

    /*!
      \qmlproperty PieChart PieChartControl::chart
     */
    property alias chart: pie

    /*!
      \qmlproperty int PieChartControl::highlight
     */
    property alias highlight: pie.highlight

    /*!
      \qmlproperty string PieChartControl::text
     */
    property alias text: centerText.text

    implicitWidth: Theme.gridUnit * 5
    implicitHeight: Theme.gridUnit * 5

    contentItem: Item {
        Charts.PieChart {
            id: pie

            anchors.fill: parent

            nameSource: Charts.ArraySource { id: nameSource; array: [ ] }
            colorSource: Charts.ColorGradientSource { id: colorSource; baseColor: Theme.highlightColor; itemCount: pie.valueSources.length }
        }

        Label {
            id: centerText

            anchors.centerIn: parent;

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
    }
}
