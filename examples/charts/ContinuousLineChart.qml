import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts

Kirigami.Page {
    title: "Line Chart"

    ListModel {
        id: lineModel;
        dynamicRoles: true;
    }

    Timer {
        id: updateTimer
        running: true
        repeat: true
        interval: 16
        property real lastValue: 0.0
        onTriggered: {
            var value = Math.max(0.0, Math.min(1.0, lastValue + (-0.05 + Math.random() / 10)))
            lastValue = value;
            lineModel.insert(0, {"value": value})
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.largeSpacing

        Kirigami.AbstractCard {
            Layout.fillHeight: false
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Charts.LineChart {
                id: chart
                anchors.fill: parent

                xRange {
                    from: 0
                    to: 500
                    automatic: false
                }

                yRange {
                    from: 0
                    to: 1
                    automatic: false
                }

                valueSources: Charts.ModelSource { roleName: "value"; model: lineModel }

                lineWidth: 2
                lineColorSource: Charts.SingleValueSource { value: "darkRed" }
                fillOpacity: 0.2
            }
        }

        RowLayout {
            RangeEditor { label: "X Axis"; range: chart.xRange }
            Item { Layout.fillWidth: true }
            Button { icon.name: "media-playback-start"; enabled: !updateTimer.running; onClicked: updateTimer.start() }
            Button { icon.name: "media-playback-stop"; enabled: updateTimer.running; onClicked: updateTimer.stop() }
            Item { Layout.fillWidth: true }
            RangeEditor { label: "Y Axis"; range: chart.yRange }
        }
    }
}
