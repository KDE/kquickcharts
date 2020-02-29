/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami

import org.kde.quickcharts 1.0 as Charts

RowLayout {
    property alias label: labelItem.text;
    property Charts.Range range;

    spacing: Kirigami.Units.smallSpacing

    Label {
        id: labelItem;
    }
    CheckBox {
        id: automaticCheckbox;
        checked: range.automatic;
        text: "Automatic";
        onToggled: range.automatic = checked
    }
    Label {
        text: "From"
    }
    SpinBox {
        from: -10000;
        to: 10000;
        value: range.from;
        editable: true;
        enabled: !automaticCheckbox.checked;
        onValueModified: range.from = value;
    }
    Label {
        text: "To"
    }
    SpinBox {
        from: -10000;
        to: 10000;
        value: range.to;
        editable: true;
        enabled: !automaticCheckbox.checked;
        onValueModified: range.to = value;
    }
    Label {
        text: "Minimum"
    }
    SpinBox {
        from: 0
        to: 10000
        value: range.minimum
        editable: true
        onValueModified: range.minimum = value
    }
    Label {
        text: "Increment"
    }
    SpinBox {
        from: 0
        to: 10000
        value: range.increment
        editable: true
        onValueModified: range.increment = value
    }
}
