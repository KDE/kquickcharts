
set(quickcharts_SRCS
    ChartsPlugin.cpp

    PieChart.cpp
    XYChart.cpp
    LineChart.cpp

    RangeGroup.cpp

    decorations/GridLines.cpp
    decorations/AxisLabels.cpp
    decorations/LegendModel.cpp

    datasource/ChartDataSource.cpp
    datasource/ModelSource.cpp
    datasource/SingleValueSource.cpp
    datasource/ArraySource.cpp
    datasource/ModelHistorySource.cpp
    datasource/ChartAxisSource.cpp

    scenegraph/PieChartMaterial.cpp
    scenegraph/PieChartNode.cpp
    scenegraph/LineGridNode.cpp
    scenegraph/LineChartNode.cpp
    scenegraph/LineChartMaterial.cpp
    scenegraph/LineSegmentNode.cpp
    scenegraph/SDFShader.cpp
)

qt5_add_resources(quickcharts_QRC shaders/shaders.qrc)

add_library(quickchartsplugin SHARED ${quickcharts_SRCS} ${quickcharts_QRC})
target_link_libraries(quickchartsplugin
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::Gui
)

install(TARGETS quickchartsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts)
