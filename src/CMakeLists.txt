
set(quickcharts_SRCS
    Chart.cpp
    XYChart.cpp
    PieChart.cpp
    LineChart.cpp
    BarChart.cpp

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
    datasource/ValueHistorySource.cpp
    datasource/ColorGradientSource.cpp

    scenegraph/PieChartMaterial.cpp
    scenegraph/PieChartNode.cpp
    scenegraph/LineGridNode.cpp
    scenegraph/LineChartNode.cpp
    scenegraph/LineChartMaterial.cpp
    scenegraph/LineSegmentNode.cpp
    scenegraph/SDFShader.cpp
    scenegraph/BarChartNode.cpp
)

qt5_add_resources(quickcharts_QRC shaders/shaders.qrc)

add_definitions(-DQT_NO_KEYWORDS)

add_library(QuickChartsStatic STATIC ${quickcharts_SRCS} ${quickcharts_QRC})
target_link_libraries(QuickChartsStatic PUBLIC Qt5::Core Qt5::Quick Qt5::Qml Qt5::Quick)

add_library(QuickCharts SHARED ChartsPlugin.cpp)
target_link_libraries(QuickCharts PRIVATE QuickChartsStatic)

install(TARGETS QuickCharts EXPORT KF5QuickChartsTargets DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts)

if (BUILD_TESTING)
    add_custom_command(TARGET QuickCharts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/org/kde/quickcharts
    )
    add_custom_command(TARGET QuickCharts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/qmldir ${CMAKE_BINARY_DIR}/bin/org/kde/quickcharts
    )
    add_custom_command(TARGET QuickCharts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/bin/libQuickCharts.so ${CMAKE_BINARY_DIR}/bin/org/kde/quickcharts/libQuickCharts.so
    )
endif()
