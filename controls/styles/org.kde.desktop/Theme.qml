import QtQuick 2.9

import org.kde.kirigami 2.4 as Kirigami

pragma Singleton;

QtObject {
    property real gridUnit: Kirigami.Units.gridUnit
    property real smallSpacing: Kirigami.Units.smallSpacing
    property real largeSpacing: Kirigami.Units.largeSpacing
}
