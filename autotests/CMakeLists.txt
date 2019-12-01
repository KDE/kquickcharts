
find_package(Qt5 COMPONENTS QuickTest)

add_executable(qmltest qmltest.cpp)
target_link_libraries(qmltest Qt5::QuickTest)

add_test(NAME BarChart COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest -input tst_BarChart.qml)
add_test(NAME LineChart COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest -input tst_LineChart.qml)
add_test(NAME PieChart COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest -input tst_PieChart.qml)

set_tests_properties(
    BarChart
    LineChart
    PieChart
    PROPERTIES ENVIRONMENT "QML2_IMPORT_PATH=${CMAKE_BINARY_DIR}/bin"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
