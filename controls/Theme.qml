import QtQuick 2.9

pragma Singleton;

QtObject {
    property real gridUnit: 20
    property real smallSpacing: 5
    property real largeSpacing: 10

    property color highlightColor: "blue"
    property color backgroundColor: "white"
}
