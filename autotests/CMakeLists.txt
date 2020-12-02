
find_package(Qt5 COMPONENTS QuickTest)

include(ECMAddTests)

include_directories(${CMAKE_SOURCE_DIR}/src)

ecm_add_tests(
    ArraySourceTest.cpp
    MapProxySourceTest.cpp
    HistoryProxySourceTest.cpp
    ItemBuilderTest.cpp
    LINK_LIBRARIES QuickChartsStatic Qt5::Test
)

add_executable(qmltest qmltest.cpp)
target_link_libraries(qmltest Qt5::QuickTest)

macro(add_qml_tests)
    if (WIN32)
        set(_extra_args -platform offscreen)
    endif()

    foreach(test ${ARGV})
        add_test(NAME ${test}
                 COMMAND ${CMAKE_BINARY_DIR}/bin/qmltest
                         ${_extra_args}
                         -import ${CMAKE_BINARY_DIR}/bin
                         -input ${test}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach()
endmacro()

add_qml_tests(
    tst_BarChart.qml
    tst_LineChart.qml
    tst_PieChart.qml
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
