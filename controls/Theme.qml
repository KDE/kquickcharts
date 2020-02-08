/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.9

pragma Singleton;

QtObject {
    property real gridUnit: 20
    property real smallSpacing: 5
    property real largeSpacing: 10

    property color highlightColor: "blue"
    property color backgroundColor: "white"
}
