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

        Component.onCompleted: {
            append({label: "1", value1: 10, value2: 15, value3: 20})
            append({label: "2", value1: 15, value2: 25, value3: 25})
            append({label: "3", value1: 15, value2: 20, value3: 30})
            append({label: "4", value1: 10, value2: 10, value3: 35})
            append({label: "5", value1: 20, value2: 5, value3: 40})
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
                    to: 10
                    automatic: true
                }

                yRange {
                    from: 0
                    to: 100
                    automatic: true
                }

                valueSources: [
                    Charts.ModelSource { roleName: "value1"; model: lineModel },
                    Charts.ModelSource { roleName: "value2"; model: lineModel },
                    Charts.ModelSource { roleName: "value3"; model: lineModel }
                ]

                lineColorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
            }
        }

        RowLayout {
            RangeEditor { label: "X Axis"; range: chart.xRange }
            Item { Layout.fillWidth: true }
            RangeEditor { label: "Y Axis"; range: chart.yRange }
        }

        RowLayout {
            Button { text: "Add Item"; onClicked: lineModel.append({label: "New", value1: 0, value2: 0, value3: 0}) }
            Button { text: "Remove Item"; onClicked: lineModel.remove(lineModel.count - 1)}
            Label { text: "Line Width" }
            SpinBox { from: 0; to: 1000; value: chart.lineWidth; onValueModified: chart.lineWidth = value; }
//             CheckBox { checked: chart.range.automatic; text: "Automatic"; onToggled: chart.range.automatic = checked }
            Label { text: "Fill Opacity" }
            SpinBox { from: 0; to: 100; value: chart.fillOpacity * 100; onValueModified: chart.fillOpacity = value / 100; }
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
        }
    }
}
