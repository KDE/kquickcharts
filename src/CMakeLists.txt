
set(quickcharts_SRCS
    BarChart.cpp
    BarChart.h
    Chart.cpp
    Chart.h
    datasource/ArraySource.cpp
    datasource/ArraySource.h
    datasource/ChartAxisSource.cpp
    datasource/ChartAxisSource.h
    datasource/ChartDataSource.cpp
    datasource/ChartDataSource.h
    datasource/ColorGradientSource.cpp
    datasource/ColorGradientSource.h
    datasource/HistoryProxySource.cpp
    datasource/HistoryProxySource.h
    datasource/MapProxySource.cpp
    datasource/MapProxySource.h
    datasource/ModelHistorySource.cpp
    datasource/ModelHistorySource.h
    datasource/ModelSource.cpp
    datasource/ModelSource.h
    datasource/SingleValueSource.cpp
    datasource/SingleValueSource.h
    datasource/ValueHistorySource.cpp
    datasource/ValueHistorySource.h
    decorations/AxisLabels.cpp
    decorations/AxisLabels.h
    decorations/GridLines.cpp
    decorations/GridLines.h
    decorations/LegendLayout.cpp
    decorations/LegendLayout.h
    decorations/LegendModel.cpp
    decorations/LegendModel.h
    ItemBuilder.cpp
    ItemBuilder.h
    LineChart.cpp
    LineChart.h
    PieChart.cpp
    PieChart.h
    QmlDeprecated.h
    RangeGroup.cpp
    RangeGroup.h
    scenegraph/BarChartMaterial.cpp
    scenegraph/BarChartMaterial.h
    scenegraph/BarChartNode.cpp
    scenegraph/BarChartNode.h
    scenegraph/LineChartMaterial.cpp
    scenegraph/LineChartMaterial.h
    scenegraph/LineChartNode.cpp
    scenegraph/LineChartNode.h
    scenegraph/LineGridNode.cpp
    scenegraph/LineGridNode.h
    scenegraph/LineSegmentNode.cpp
    scenegraph/LineSegmentNode.h
    scenegraph/PieChartMaterial.cpp
    scenegraph/PieChartMaterial.h
    scenegraph/PieChartNode.cpp
    scenegraph/PieChartNode.h
    scenegraph/SDFShader.cpp
    scenegraph/SDFShader.h
    XYChart.cpp
    XYChart.h
)

qt5_add_resources(quickcharts_QRC shaders/shaders.qrc)

ecm_qt_declare_logging_category(quickcharts_SRCS
    HEADER charts_general_logging.h
    IDENTIFIER GENERAL
    CATEGORY_NAME kf.quickcharts.general
    DEFAULT_SEVERITY Warning
    DESCRIPTION "KQuickCharts Library - General"
    EXPORT KQuickCharts
)

ecm_qt_declare_logging_category(quickcharts_SRCS
    HEADER charts_datasource_logging.h
    IDENTIFIER DATASOURCE
    CATEGORY_NAME kf.quickcharts.datasource
    DEFAULT_SEVERITY Warning
    DESCRIPTION "KQuickCharts Library - Data Sources"
    EXPORT KQuickCharts
)

ecm_qt_declare_logging_category(quickcharts_SRCS
    HEADER charts_deprecated_logging.h
    IDENTIFIER DEPRECATED
    CATEGORY_NAME kf.quickcharts.deprecated
    DEFAULT_SEVERITY Warning
    DESCRIPTION "KQuickCharts Library - Deprecated API"
    EXPORT KQuickCharts
)

add_library(QuickChartsStatic STATIC ${quickcharts_SRCS} ${quickcharts_QRC})
target_link_libraries(QuickChartsStatic PUBLIC Qt5::Core Qt5::Quick Qt5::Qml Qt5::Quick)

add_library(QuickCharts SHARED ChartsPlugin.cpp ChartsPlugin.h)
target_link_libraries(QuickCharts PRIVATE QuickChartsStatic)

ecm_generate_export_header(QuickCharts
    BASE_NAME QuickCharts
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.78
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

install(TARGETS QuickCharts EXPORT KF5QuickChartsTargets DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts)

ecm_qt_install_logging_categories(EXPORT KQuickCharts DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

if (BUILD_TESTING)
    add_custom_command(TARGET QuickCharts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/org/kde/quickcharts
    )
    add_custom_command(TARGET QuickCharts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/qmldir ${CMAKE_BINARY_DIR}/bin/org/kde/quickcharts
    )
    add_custom_command(TARGET QuickCharts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:QuickCharts> ${CMAKE_BINARY_DIR}/bin/org/kde/quickcharts
    )
endif()
