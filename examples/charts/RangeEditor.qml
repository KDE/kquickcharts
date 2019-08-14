/*
 * This file is part of Quick Charts.
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
