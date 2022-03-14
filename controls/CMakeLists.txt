
ecm_add_qml_module(chartscontrolsplugin URI "org.kde.quickcharts.controls")

target_sources(chartscontrolsplugin PRIVATE ChartsControlsPlugin.cpp)

ecm_target_qml_sources(chartscontrolsplugin SOURCES
    Legend.qml
    LegendDelegate.qml
    LineChartControl.qml
    PieChartControl.qml
    Theme.qml
    Logging.qml
)

ecm_target_qml_sources(chartscontrolsplugin PRIVATE PATH styles/org.kde.desktop SOURCES
    styles/org.kde.desktop/Theme.qml
)

target_link_libraries(chartscontrolsplugin
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::QuickControls2
)

ecm_finalize_qml_module(chartscontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR})
