
ecm_add_qml_module(QuickChartsControls URI "org.kde.quickcharts.controls")

target_sources(QuickChartsControls PRIVATE ChartsControlsPlugin.cpp)

ecm_target_qml_sources(QuickChartsControls SOURCES
    Legend.qml
    LegendDelegate.qml
    LineChartControl.qml
    PieChartControl.qml
    Theme.qml
    Logging.qml
)

ecm_target_qml_sources(QuickChartsControls PRIVATE PATH styles/org.kde.desktop SOURCES
    styles/org.kde.desktop/Theme.qml
)

target_link_libraries(QuickChartsControls
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::QuickControls2
)

ecm_finalize_qml_module(QuickChartsControls DESTINATION ${KDE_INSTALL_QMLDIR})
