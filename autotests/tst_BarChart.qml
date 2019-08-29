import QtQuick 2.11
import QtTest 1.11

import org.kde.quickcharts 1.0 as Charts

TestCase {
    id: testCase
    name: "Bar Chart Tests"

    Component {
        id: minimal
        Charts.BarChart { }
    }

    Component {
        id: simple
        Charts.BarChart {
            width: 200
            height: 200
            nameSource: Charts.ArraySource { array: ["Test 1", "Test 2", "Test 3"] }
            colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
            valueSources: Charts.ArraySource { array: [1, 2, 3, 4, 5] }
        }
    }

    function test_create_data() {
        return [
            { tag: "minimal", component: minimal },
            { tag: "simple", component: simple }
        ]
    }

    function test_create(data) {
        var item = createTemporaryObject(data.component, testCase)
        verify(item)
    }
}


