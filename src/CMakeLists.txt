
set(quickcharts_SRCS
    Chart.cpp
    XYChart.cpp
    PieChart.cpp
    LineChart.cpp
    BarChart.cpp

    RangeGroup.cpp
    ItemBuilder.cpp

    decorations/GridLines.cpp
    decorations/AxisLabels.cpp
    decorations/LegendModel.cpp
    decorations/LegendLayout.cpp

    datasource/ChartDataSource.cpp
    datasource/ModelSource.cpp
    datasource/SingleValueSource.cpp
    datasource/ArraySource.cpp
    datasource/ModelHistorySource.cpp
    datasource/ChartAxisSource.cpp
    datasource/ValueHistorySource.cpp
    datasource/ColorGradientSource.cpp
    datasource/MapProxySource.cpp
    datasource/HistoryProxySource.cpp

    scenegraph/PieChartMaterial.cpp
    scenegraph/PieChartNode.cpp
    scenegraph/LineGridNode.cpp
    scenegraph/LineChartNode.cpp
    scenegraph/LineChartMaterial.cpp
    scenegraph/LineSegmentNode.cpp
    scenegraph/SDFShader.cpp
    scenegraph/BarChartNode.cpp
    scenegraph/BarChartMaterial.cpp
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

add_library(QuickCharts SHARED ChartsPlugin.cpp)
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
