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
                anchors.fill: parent
                anchors.margins: Kirigami.Units.smallSpacing;

                innerDiameter: 250

                valueSource: Charts.ModelRole { name: "data"; model: pieModel }
                colorSource: Charts.ModelRole { name: "color"; model: pieModel }
            }
        }

        RowLayout {
            Button { text: "Add Item"; onClicked: pieModel.append({data: 50, color: Qt.rgba(1.0, 1.0, 1.0)}) }
            Button { text: "Remove Item"; onClicked: pieModel.remove(pieModel.count - 1)}
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
