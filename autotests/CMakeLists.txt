
find_package(Qt5 COMPONENTS QuickTest)

include(ECMAddTests)

include_directories(${CMAKE_SOURCE_DIR}/src)

ecm_add_tests(
    ArraySourceTest.cpp
    LINK_LIBRARIES QuickChartsStatic Qt5::Test
)

add_executable(qmltest qmltest.cpp)
target_link_libraries(qmltest Qt5::QuickTest)

add_test(
    NAME BarChart
    COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest -import ${CMAKE_BINARY_DIR}/bin -input tst_BarChart.qml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(
    NAME LineChart
    COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest -import ${CMAKE_BINARY_DIR}/bin -input tst_LineChart.qml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(
    NAME PieChart
    COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest -import ${CMAKE_BINARY_DIR}/bin -input tst_PieChart.qml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

find_program(GLSL_VALIDATOR NAMES glslangValidator)
if (GLSL_VALIDATOR)
    add_test(
        NAME ShaderValidation
        COMMAND ${CMAKE_SOURCE_DIR}/scripts/validate_shaders.sh ${CMAKE_SOURCE_DIR}/src/shaders
    )
else()
    message(WARNING "glslangValidator not found, cannot validate shaders")
endif()
