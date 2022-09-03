
find_package(Qt${QT_MAJOR_VERSION}Widgets REQUIRED)
set_package_properties(Qt${QT_MAJOR_VERSION}Widgets PROPERTIES TYPE REQUIRED PURPOSE "The charts example needs Qt Widgets so the Plasma desktop style works properly.")
find_package(KF5Kirigami2 REQUIRED)
set_package_properties(KF5Kirigami2 PROPERTIES TYPE REQUIRED PURPOSE "The charts example uses Kirigami for its UI.")
find_package(KF5Declarative REQUIRED)
set_package_properties(KF5Declarative PROPERTIES TYPE REQUIRED PURPOSE "The charts example uses items from KF5Declarative for its UI.")

set(quickcharts_example_SRCS
    main.cpp
)

qt5_add_resources(quickcharts_example_QRC charts.qrc)

add_executable(kquickcharts_example ${quickcharts_example_SRCS} ${quickcharts_example_QRC})
target_link_libraries(kquickcharts_example Qt5::Quick Qt5::Widgets)

install(TARGETS kquickcharts_example DESTINATION ${KDE_INSTALL_BINDIR})
