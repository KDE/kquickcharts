
find_package(Qt5 COMPONENTS QuickTest)

add_test(NAME BarChart COMMAND qmltestrunner -input tst_BarChart.qml WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME LineChart COMMAND qmltestrunner -input tst_LineChart.qml WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME PieChart COMMAND qmltestrunner -input tst_PieChart.qml WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_tests_properties(
    BarChart
    LineChart
    PieChart
    PROPERTIES ENVIRONMENT "QML2_IMPORT_PATH=${CMAKE_BINARY_DIR}/bin"
)
