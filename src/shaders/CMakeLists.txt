# SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(_extra_args DEBUGINFO)
else()
    set(_extra_args OPTIMIZED)
endif()

qt6_add_shaders(QuickCharts "shaders"
    BATCHABLE
    ZORDER_LOC 6
    PRECOMPILE
    PREFIX "/org.kde.quickcharts/"
    GLSL "300es,130"
    FILES
        barchart.vert
        barchart.frag
        linechart.vert
        linechart.frag
        piechart.vert
        piechart.frag
    ${_extra_args}
)
