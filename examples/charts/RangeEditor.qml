import QtQuick 2.9
import QtQuick.Controls 2.2
import org.kde.kirigami 2.4 as Kirigami

import org.kde.quickcharts 1.0 as Charts

Row {
    property alias label: labelItem.text;
    property Charts.Range range;

    spacing: Kirigami.Units.smallSpacing

    Label {
        id: labelItem;
        anchors.verticalCenter: parent.verticalCenter;
        text: "X Axis";
    }
    CheckBox {
        id: automaticCheckbox;
        anchors.verticalCenter: parent.verticalCenter;
        checked: range.automatic;
        text: "Automatic";
        onToggled: range.automatic = checked
    }
    Label {
        anchors.verticalCenter: parent.verticalCenter;
        text: "From"
    }
    SpinBox {
        anchors.verticalCenter: parent.verticalCenter;
        from: -10000;
        to: 10000;
        value: range.from;
        editable: true;
        enabled: !automaticCheckbox.checked;
        onValueModified: range.from = value;
    }
    Label {
        anchors.verticalCenter: parent.verticalCenter;
        text: "To"
    }
    SpinBox {
        anchors.verticalCenter: parent.verticalCenter;
        from: -10000;
        to: 10000;
        value: range.to;
        editable: true;
        enabled: !automaticCheckbox.checked;
        onValueModified: range.to = value;
    }
}
