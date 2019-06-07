import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrols 2.0

import org.kde.quickcharts 1.0 as Charts

Kirigami.Page {
    title: "Pie Chart"

    ListModel {
        id: pieModel;
        dynamicRoles: true;

        Component.onCompleted: {
            append({ data: 50, color: "red" })
            append({ data: 50, color: "green" })
            append({ data: 50, color: "blue" })
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.largeSpacing

        Kirigami.AbstractCard {
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.preferredWidth: 400
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Charts.PieChart {
                id: chart
                anchors.fill: parent
                anchors.margins: Kirigami.Units.smallSpacing;

                borderWidth: 20;
                range.to: 150

                valueSource: Charts.ModelSource { roleName: "data"; model: pieModel }
                colorSource: Charts.ModelSource { roleName: "color"; model: pieModel }

                backgroundColor: "gray"
            }
        }

        RowLayout {
            Button { text: "Add Item"; onClicked: pieModel.append({data: 50, color: Qt.rgba(1.0, 1.0, 1.0)}) }
            Button { text: "Remove Item"; onClicked: pieModel.remove(pieModel.count - 1)}
            Label { text: "Border" }
            SpinBox { from: 0; to: chart.width / 2; value: chart.borderWidth; onValueModified: chart.borderWidth = value; }
            RangeEditor { label: "Range"; range: chart.range }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                ListView {
                    model: pieModel;
                    delegate: Kirigami.BasicListItem {
                        width: ListView.view.width
                        height: Kirigami.Units.gridUnit * 2 + Kirigami.Units.smallSpacing
                        contentItem: RowLayout {
                            Label { text: "Value" }
                            SpinBox {
                                Layout.preferredWidth: parent.width * 0.15
                                from: 0; to: 10000;
                                stepSize: 1;
                                value: model.data;
                                onValueModified: pieModel.setProperty(index, "data", value)
                            }
                            Label { text: "Color" }
                            ColorButton {
                                color: model.color;
                                showAlphaChannel: true;
                                onColorChanged: pieModel.setProperty(index, "color", color)
                            }
                        }
                    }
                }
            }
        }
    }
}
