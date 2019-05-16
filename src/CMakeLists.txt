
set(quickcharts_SRCS
    ChartsPlugin.cpp
    PieChart.cpp
    PieChartNode.cpp
    PieChartMaterial.cpp
    DataSource.cpp
    ModelRole.cpp
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
