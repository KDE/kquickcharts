

add_library(chartscontrolsplugin SHARED ChartsControlsPlugin.cpp)
target_link_libraries(chartscontrolsplugin
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::QuickControls2
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
