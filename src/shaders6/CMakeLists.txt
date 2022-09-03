# SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

qt6_add_shaders(QuickCharts "shaders6"
    BATCHABLE
    PRECOMPILE
    OPTIMIZED
    PREFIX "/org.kde.quickcharts/"
    GLSL "300es,130"
    FILES
        barchart.vert
        barchart.frag
        linechart.vert
        linechart.frag
        piechart.vert
        piechart.frag
)
