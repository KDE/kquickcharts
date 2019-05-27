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
        running: true
        repeat: true
        interval: 17
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

                lineColorSource: Charts.SingleValueSource { value: "darkRed" }
                fillOpacity: 0.2
            }
        }

        RowLayout {
            RangeEditor { label: "X Axis"; range: chart.xRange }
            Item { Layout.fillWidth: true }
            RangeEditor { label: "Y Axis"; range: chart.yRange }
        }

       /* RowLayout {
            Button { text: "Add Item"; onClicked: lineModel.append({label: "New", value1: 0, value2: 0, value3: 0}) }
            Button { text: "Remove Item"; onClicked: lineModel.remove(lineModel.count - 1)}
//             Label { text: "Border" }
//             SpinBox { from: 0; to: chart.width / 2; value: chart.borderWidth; onValueModified: chart.borderWidth = value; }
//             CheckBox { checked: chart.range.automatic; text: "Automatic"; onToggled: chart.range.automatic = checked }
//             Label { text: "From" }
//             SpinBox { from: 0; to: 10000; value: chart.range.from; onValueModified: chart.range.from = value; }
//             Label { text: "To" }
//             SpinBox { from: -1; to: 10000; value: chart.range.to; onValueModified: chart.range.to = value; }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                ListView {
                    model: lineModel;
                    delegate: Kirigami.BasicListItem {
                        width: ListView.view.width
                        height: Kirigami.Units.gridUnit * 2 + Kirigami.Units.smallSpacing
                        contentItem: RowLayout {
                            Label { text: "Label" }
                            TextField { text: model.label; onEditingFinished: lineModel.setProperty(index, "label", text) }
                            Label { text: "Value 1" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.value1;
                                onValueModified: lineModel.setProperty(index, "value1", value)
                            }
                            Label { text: "Value 2" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.value2;
                                onValueModified: lineModel.setProperty(index, "value2", value)
                            }
                            Label { text: "Value 3" }
                            SpinBox {
                                Layout.preferredWidth: 75
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.value3;
                                onValueModified: lineModel.setProperty(index, "value3", value)
                            }
                        }
                    }
                }
            }
        }*/
    }
}
