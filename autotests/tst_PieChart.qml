/*
 * This file is part of KQuickCharts
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtTest 1.11

import org.kde.quickcharts 1.0 as Charts

TestCase {
    id: testCase
    name: "Pie Chart Tests"

    Component {
        id: minimal
        Charts.PieChart { }
    }

    Component {
        id: simple
        Charts.PieChart {
            width: 200
            height: 200
            nameSource: Charts.ArraySource { array: ["Test 1", "Test 2", "Test 3"] }
            colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
            valueSources: Charts.ArraySource { array: [1, 2, 3, 4, 5] }
        }
    }

    Component {
        id: multiValue
        Charts.PieChart {
            width: 200
            height: 200
            nameSource: Charts.ArraySource { array: ["Test 1", "Test 2", "Test 3"] }
            colorSource: Charts.ArraySource { array: ["red", "green", "blue"] }
            valueSources: [
                Charts.ArraySource { array: [1, 2, 3, 4, 5] },
                Charts.ArraySource { array: [1, 2, 3, 4, 5] },
                Charts.ArraySource { array: [1, 2, 3, 4, 5] }
            ]
        }
    }

    Component {
        id: model
        Charts.PieChart {
            width: 200
            height: 200

            valueSources: [
                Charts.ModelSource {
                    model: ListModel {
                        id: listModel
                        ListElement { name: "Test 1"; color: "red"; value: 1 }
                        ListElement { name: "Test 2"; color: "green"; value: 2 }
                        ListElement { name: "Test 3"; color: "blue"; value: 3 }
                        ListElement { name: "Test 4"; color: "cyan"; value: 4 }
                        ListElement { name: "Test 5"; color: "magenta"; value: 5 }
                        ListElement { name: "Test 6"; color: "yellow"; value: 6 }
                    }
                }
            ]
            nameSource: Charts.ModelSource { model: listModel; roleName: "name" }
            colorSource: Charts.ModelSource { model: listModel; roleName: "color" }
        }
    }

    function test_create_data() {
        return [
            { tag: "minimal", component: minimal },
            { tag: "simple", component: simple },
            { tag: "multiValue", component: multiValue },
            { tag: "model", component: model }
        ]
    }

    function test_create(data) {
        var item = createTemporaryObject(data.component, testCase)
        verify(item)
    }
}

