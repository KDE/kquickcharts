

add_library(chartscontrolsplugin SHARED ChartsControlsPlugin.cpp)
target_link_libraries(chartscontrolsplugin
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::Gui
    Qt5::QuickControls2
)

install(TARGETS chartscontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts/controls)
install(FILES
    qmldir
    Legend.qml
    LegendDelegate.qml
    LineChartControl.qml
    PieChartControl.qml
    Theme.qml
    Logging.qml
    DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts/controls
)

install(DIRECTORY styles DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/quickcharts/controls)
